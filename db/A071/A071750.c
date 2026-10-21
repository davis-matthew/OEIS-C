/*
 * A071750  Numbers k such that 13 divides p(k), the k-th partition number,
 *          A000041(k).
 * https://oeis.org/A071750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A071750: numbers k such that 13 divides the k‑th partition number p(k).
   The function builds the partition numbers modulo 13 using Euler's
   pentagonal number theorem.  Whenever a residue 0 occurs the index k is
   stored.  The nth stored index is returned as the nth sequence value. */
#include <stdlib.h>
#include <stdint.h>

/* static tables of partition residues modulo 13 and of the requested k’s */
static uint8_t *p = NULL;          /* p[0] = 1 (partition of 0)          */
static idx_t   p_size = 0, p_cap = 0;
static seq_t *zero = NULL;         /* indices k with p(k) % 13 == 0       */
static idx_t  zero_size = 0, zero_cap = 0;
static idx_t  last_requested = 0;  /* maximum n asked for so far         */

/* helper to grow a table of uint8_t */
static void grow_uint8(void) {
    if (p_size == p_cap) {
        p_cap = p_cap ? p_cap * 2 : 16;
        p = (uint8_t *)realloc(p, p_cap * sizeof(uint8_t));
    }
}
/* helper to grow a table of seq_t */
static void grow_seq(void) {
    if (zero_size == zero_cap) {
        zero_cap = zero_cap ? zero_cap * 2 : 16;
        zero = (seq_t *)realloc(zero, zero_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n) {
    const int MOD = 13;
    if (n == 0) return 0;             /* offset starts at 1, but guard                */
    if (n > last_requested)           /* need more terms */
        last_requested = n;

    /* initialise tables if this is the first call */
    if (p_size == 0) {
        p_cap = 16;
        p = (uint8_t *)malloc(p_cap * sizeof(uint8_t));
        p[0] = 1;                     /* p(0) = 1                              */
        p_size = 1;
    }

    /* generate partition residues until we have enough zeroes */
    while (zero_size < last_requested) {
        idx_t k = p_size;              /* we are computing p(k)                    */

        int val = 0;
        int sign = 1;
        /* generalized pentagonal numbers g(i) = i(3i-1)/2  and  i(3i+1)/2  */
        for (int i = 1; ; ++i) {
            int64_t g1 = (int64_t)i * (3LL * i - 1) / 2;
            if (g1 > k) break;
            val += sign * p[(size_t)(k - g1)];
            int64_t g2 = (int64_t)i * (3LL * i + 1) / 2;
            if (g2 <= k)
                val += sign * p[(size_t)(k - g2)];
            sign = -sign;
        }

        val %= MOD;
        if (val < 0) val += MOD;

        /* append to partition table */
        grow_uint8();
        p[p_size++] = (uint8_t)val;

        /* if zero, record the index */
        if (val == 0) {
            grow_seq();
            zero[zero_size++] = (seq_t)k;
        }
    }

    return zero[(size_t)(n - 1)];
}

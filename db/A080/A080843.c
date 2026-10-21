/*
 * A080843  Tribonacci word: limit S(infinity), where S(0) = 0, S(1) = 0,1,
 *          S(2) = 0,1,0,2 and for n >= 0, S(n+3) = S(n+2) S(n+1) S(n).
 * https://oeis.org/A080843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8912, u16 n <= 8912, u32 n <= 8912, u64 n <= 8912,
 *            u128 n <= 8912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Tribonacci word symbol at position n.
   Defined by S(0)=0, S(1)=01, S(2)=0102,
   and for k≥0: S(k+3)=S(k+2)S(k+1)S(k).
   Initial terms: 0,1,0,2,0,1,0,0,1,0,2,0,1,0,1,0,2,0,1,0,0,1,0,2,...   */

#include <stdlib.h>
#include <stdint.h>

/* Array storing the lengths of S(k).  All values fit into 64‑bit for n up
   to at least 10⁶, the test harness never needs bigger lengths.          */
static uint64_t *lens = NULL;
static size_t lens_cnt   = 0;   /* number of stored lengths                */
static size_t lens_cap   = 0;   /* current allocation size                */

/* Ensure that lens contains all Lk with Lk > n.  */
static void ensure_lengths(idx_t n)
{
    uint64_t target = (uint64_t)n;
    while (lens_cnt == 0 || lens[lens_cnt-1] <= target) {
        /* Grow the array when necessary.  */
        if (lens_cnt == lens_cap) {
            size_t new_cap = lens_cap ? lens_cap << 1 : 8;
            uint64_t *tmp = (uint64_t*)realloc(lens, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);          /* out of memory */
            lens = tmp;
            lens_cap = new_cap;
        }

        /* Generate the next Tribonacci length.   */
        if (lens_cnt == 0)
            lens[lens_cnt++] = 1;          /* L0 = 1  (S0 has 1 symbol)  */
        else if (lens_cnt == 1)
            lens[lens_cnt++] = 2;          /* L1 = 2  (S1 has 2 symbols)  */
        else if (lens_cnt == 2)
            lens[lens_cnt++] = 4;          /* L2 = 4  (S2 has 4 symbols)  */
        else {
            uint64_t a = lens[lens_cnt-1];
            uint64_t b = lens[lens_cnt-2];
            uint64_t c = lens[lens_cnt-3];
            /* The sum fits in 64 bits for the needed range of n.   */
            lens[lens_cnt++] = a + b + c;
        }
    }
}

/* Public function requested by the test harness. */
seq_t a(idx_t n)
{
    ensure_lengths(n);

    /* Find the smallest k such that Lk > n. */
    size_t k = lens_cnt - 1;
    uint64_t pos = (uint64_t)n;

    /* Walk backwards from S(k) to the base words S(0), S(1), S(2). */
    while (k > 2) {
        uint64_t len1 = lens[k-1];
        uint64_t len2 = lens[k-2];
        if (pos < len1) {
            k -= 1;
        } else if (pos < len1 + len2) {
            pos -= len1;
            k -= 2;
        } else {
            pos -= len1 + len2;
            k -= 3;
        }
    }

    /* Base cases are small; return the appropriate symbol. */
    switch (k) {
        case 0:                 /* S0 = [0]          */
            return (seq_t)0;
        case 1:                 /* S1 = [0,1]        */
            return (seq_t)(pos == 0 ? 0 : 1);
        case 2:                 /* S2 = [0,1,0,2]    */
            if (pos == 0) return (seq_t)0;
            if (pos == 1) return (seq_t)1;
            if (pos == 2) return (seq_t)0;
            return (seq_t)2;      /* pos == 3           */
        default:                /* unreachable          */
            return (seq_t)0;
    }
}

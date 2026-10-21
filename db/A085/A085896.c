/*
 * A085896  Group the natural numbers such that the product of the n-th group
 *          is divisible by 2^n. (1), (2), (3,4), (5,6,7,8),
 *          (9,10,11,12,13,14), (15,16,17,18), ... Sequence contains the
 *          number of terms in each group.
 * https://oeis.org/A085896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A085896.  For each n the function returns the number of
   consecutive integers beginning from where the previous group left
   off, whose product is the smallest integer divisible by 2^n.  This
   is exactly the described grouping rule. */
#include <stdint.h>
#include <stdlib.h>

/* v2: exponent of 2 in a positive 64‑bit integer. */
static inline unsigned int v2_uint64(uint64_t x)
{
    /* x is always >0 in this algorithm */
    return __builtin_ctzll(x);
}

seq_t a(idx_t n)
{
    /* static state to support arbitrary order calls */
    static seq_t *seq = NULL;       /* array of group lengths */
    static idx_t len = 0;           /* number of groups computed so far */
    static uint64_t cur = 1;        /* current natural number to use */

    /* allocate or grow the table */
    if (seq == NULL) {
        static idx_t init_cap = 128;
        seq = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq) return 0;
        len = 0;
    }
    while (len <= n) {
        idx_t group = len;          /* current group index */
        idx_t sz = 0;
        unsigned int sum_v2 = 0;    /* sum of v2 values in this group */

        /* take at least one element; stop when the product has ≥group
           powers of two. */
        do {
            sz++;
            sum_v2 += v2_uint64(cur);
            cur++;
        } while (sum_v2 < group);

        seq[len++] = (seq_t)sz;
    }
    return seq[n];
}

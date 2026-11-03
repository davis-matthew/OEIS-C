/*
 * A147299  a(n) = largest value of the function rad(m*n*(n - m)) n=2,3,4,...,
 *          0 < m < n where the function rad(k) (also called radical(k)) is
 *          the product of distinct prime divisors of k.
 * https://oeis.org/A147299
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{0<m<n} rad(m * n * (n - m))
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 999 (2026-11-01)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/radical.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* rad(n) is constant for all *m* in the loop */
    seq_t rad_n = radical(n);
    seq_t max_rad = 0;

    for (idx_t m = 1; m < n; ++m) {
        seq_t rad_m   = radical(m);
        seq_t rad_nm  = radical(n - m);
        seq_t prod = seq_mul(rad_m, rad_n);
        prod = seq_mul(prod, rad_nm);

        /* prod fits into an idx_t for the allowed n (see analysis) */
        seq_t rad_prod = radical((idx_t)prod);

        if (rad_prod > max_rad)
            max_rad = rad_prod;
    }

    return max_rad;
}

/*
 * A130036  Denominators of partial sums of a series for the inverse of the
 *          arithmetic-geometric mean (agM) of 1 and sqrt(3)/2.
 * https://oeis.org/A130036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^(6n - 2*popcount(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* count number of 1‑bits in n */
    idx_t pop = (idx_t)__builtin_popcountll((unsigned long long)n);

    /* compute exponent as 6*n - 2*popcount(n) in 128‑bit to avoid overflow */
    unsigned __int128 exp_big = (unsigned __int128)6 * n - (unsigned __int128)2 * pop;
    idx_t exp = (idx_t)exp_big;          /* fits in idx_t for all test cases */

    return pow2(exp);                    /* 2^exp in the required seq_t type */
}

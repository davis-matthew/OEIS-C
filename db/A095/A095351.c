/*
 * A095351  Total number of edges in all labeled graphs on n nodes.
 * https://oeis.org/A095351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,2) * 2^(binomial(n,2)-1) (n ≥ 2, a(1)=0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes OEIS A095351 */
seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }

    /* binomial(n,2) = n*(n-1)/2, computed in seq_t to avoid overflow */
    seq_t bin = (seq_t)n * ((seq_t)n - 1) / 2;

    /* exponent for the power of two is bin-1, which fits in idx_t
       for the n ranges considered (n ≤ 10000 → bin ≤ 49'995'000) */
    idx_t exp = (idx_t)(bin - 1);

    seq_t p = pow2(exp);          /* 2^(bin-1) */
    return bin * p;
}

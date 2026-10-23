/*
 * A152907  Irregular triangle read by rows, numbers in A007318 repeated four
 *          times .
 * https://oeis.org/A152907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   the sequence A152907 is produced by reading the rows of Pascal's
 *            triangle (A007318) and repeating each entry four times. Let r be
 *            the row index (starting at 0). Row r contains r+1 binomial
 *            coefficients C(r,k) for k=0..r. Each coefficient is repeated 4
 *            times, so the linear index n satisfies start(...
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_n_k.h"

static idx_t isqrt_u128(unsigned __int128 x)
{
    /* Fast integer square root for values ≤ 2^65-1. */
    double approx = sqrt((long double)x);
    idx_t r = (idx_t)approx;
    /* Adjust upwards */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    /* Adjust downwards */
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    /* ---- Find the row r that contains position n ---- */
    unsigned __int128 n128 = n;
    /* Compute r estimate from the inequality 2*r*(r+1) ≤ n. */
    unsigned __int128 val = (unsigned __int128)2 * n128 + 1; /* 1 + 2*n */
    idx_t sqrt_val = isqrt_u128(val);
    idx_t r_est = (sqrt_val - 1) / 2;

    /* Compute start of this row */
    unsigned __int128 start128 = (unsigned __int128)2 * r_est * (r_est + 1);
    /* Correct r_est if the estimate is slightly off */
    while (start128 > n128) {
        --r_est;
        start128 = (unsigned __int128)2 * r_est * (r_est + 1);
    }
    while (true) {
        unsigned __int128 next_start = start128 + (unsigned __int128)4 * (r_est + 1);
        if (n128 < next_start) break;
        ++r_est;
        start128 = next_start;
    }

    /* ---- Find the binomial index k within row r ---- */
    idx_t pos = (idx_t)(n128 - start128);          /* 0 ≤ pos < 4*(r+1) */
    idx_t k = pos / 4;                             /* which binomial coeff */

    /* ---- Return the binomial coefficient C(r,k) ---- */
    return binom_n_k(r_est, k);
}

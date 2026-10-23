/*
 * A038239  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*9^j.
 * https://oeis.org/A038239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ∑ (i,j) where i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 a(n) =
 *            binomial(i, j) * 4^(i-j) * 9^j, where i = largest integer with
 *            i*(i+1)/2 ≤ n and j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Power of a small integer base to an unsigned exponent.
   Works for any seq_t (fixed‑size or arbitrary‑precision). */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1U)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t lo = 0, hi = n;          /* hi is an upper bound for i */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t i = lo;
    /* Column index j within row i */
    idx_t tri_i = (__uint128_t)i * (i + 1) / 2;
    idx_t j = n - tri_i;           /* 0 ≤ j ≤ i */

    /* Compute the term: binomial(i, j) * 4^(i-j) * 9^j */
    seq_t bin = binom(i, j);
    seq_t pow4 = pow_uint((seq_t)4, i - j);
    seq_t pow9 = pow_uint((seq_t)9, j);
    return bin * pow4 * pow9;
}

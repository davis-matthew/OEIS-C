/*
 * A064382  Number of ways to put numbers 1, 2, ..., n*(n+1)/2 in a triangular
 *          array of n rows in such a way that each row is increasing or
 *          decreasing.
 * https://oeis.org/A064382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) * (n*(n+1)/2)! / (1!*2!*...*n!)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/* Compute the n-th term of OEIS sequence A064382.
   a(1) = 1, a(2) = 6, ... as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    /* Behaviour for n == 0 is irrelevant (below offset). */
    if (n == 0)
        return 0;

    /* Total number of entries in the triangular array. */
    seq_t S = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Multiply over the product of binomial coefficients,
       then multiply by 2^(n-1). */
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* sum of 1 .. (k-1) */
        idx_t sub = k * (k - 1) / 2;
        /* Numerator for this binomial coefficient. */
        seq_t N = S - (seq_t)sub;
        /* Compute binomial(N, k).  N fits in idx_t for the
           intended range of n (≤10 000). */
        seq_t bin = binomial((idx_t)N, k);
        result *= bin;
    }

    /* Multiply by 2^(n-1). */
    seq_t pow2 = pow_uint(2, n - 1);
    result *= pow2;

    return result;
}

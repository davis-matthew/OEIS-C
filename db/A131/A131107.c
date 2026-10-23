/*
 * A131107  Rectangular array read by antidiagonals: k objects are each put
 *          into one of n boxes, independently with equal probability. a(n, k)
 *          is the expected number of boxes with exactly one object (n, k >=
 *          1). Sequence gives the denominators.
 * https://oeis.org/A131107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn, tabl
 * Formula:   a(n) = denominator of the expected number of boxes with exactly
 *            one object when k objects are each put into one of n boxes
 *            independently with equal probability
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the denominators of the reduced fraction
   k * ((n-1)/n)^(k-1).  The array is read by antidiagonals
   with offset 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow_int.h"

/* Return the denominator of the reduced fraction for the
   pair (N,K) found at position n in the antidiagonal order. */
seq_t a(idx_t n)
{
    /* Find the diagonal S such that the index n lies on it.
       Length of diagonal S is S-1.  We accumulate lengths
       until we reach or surpass n. */
    idx_t cumulative = 0;   /* sum of lengths of previous diagonals */
    idx_t len = 1;          /* length of current diagonal (S-1) */

    while (cumulative + len < n)
    {
        cumulative += len;
        ++len;
    }
    idx_t S = len + 1;              /* diagonal number (N + K) */
    idx_t off = n - cumulative;     /* 1‑based position within the diagonal */

    /* In a diagonal of sum S, the pair (N,K) is
       N = S - off,          K = off              */
    idx_t N = S - off;
    idx_t K = off;

    /* Compute the reduced fraction k * (n-1)^(k-1) / n^(k-1). */
    idx_t exp = (K == 0) ? 0 : K - 1;                 /* K ≥ 1 always */
    seq_t pow_N_minus1 = pow_int((seq_t)(N - 1), exp);
    seq_t pow_N        = pow_int((seq_t)N, exp);
    seq_t numerator    = (seq_t)K * pow_N_minus1;     /* K * (N-1)^(K-1) */
    seq_t denominator  = pow_N;                       /* N^(K-1) */

    seq_t g = gcd(numerator, denominator);            /* reduce fraction */
    denominator /= g;

    return denominator;
}

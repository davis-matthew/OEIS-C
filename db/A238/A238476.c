/*
 * A238476  Rectangular array with all start numbers Mo(n, k), k >= 1, for the
 *          Collatz operation ud^(2*n-1), n >= 1, ending in an odd number,
 *          read by antidiagonals.
 * https://oeis.org/A238476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read by antidiagonals from the rectangular array *
 *            Mo(n,k) defined by: * Mo(n,k) = 2^(2*n)*k - (2^(2*n-1)+1)/3 for
 *            n,k ≥ 1. * The index n is turned into coordinates (m,k) with m+k
 *            constant, * then Mo(m,k) is returned
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

/* Return the n-th term of A238476, with 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset guard – not used by OEIS */
        return 0;

    /* Find the smallest s such that (s-1)*s/2 ≥ n
     * (s is the sum of indices in the antidiagonal).  */
    idx_t s   = 2;           /* start with first diagonal (sum=2) */
    idx_t cum = 1;           /* number of terms up to current diagonal */
    while (cum < n) {
        s   += 1;
        cum += s - 1;        /* add terms of this new diagonal */
    }

    /* Number of terms before diagonal s. */
    idx_t prev = cum - (s - 1);   /* (s-2)*(s-1)/2 */
    /* Zero‑based offset inside the diagonal. */
    idx_t r    = n - prev - 1;
    /* Coordinates in the original rectangle Mo(m,k). */
    idx_t m    = r + 1;          /* same as n1 in the derivation */
    idx_t k    = s - m;

    /* Compute Mo(m,k) using the closed formula. */
    seq_t pow2_odd = big_pow((seq_t)2, (seq_t)(2 * m - 1));  /* 2^(2*m-1) */
    seq_t pow2_even = pow2_odd << 1;                         /* 2^(2*m)   */
    seq_t div       = (pow2_odd + (seq_t)1) / (seq_t)3;      /* (2^(2*m-1)+1)/3 */
    seq_t result    = pow2_even * (seq_t)k - div;

    return result;
}

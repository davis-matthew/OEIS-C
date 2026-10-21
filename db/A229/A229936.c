/*
 * A229936  Sum of all parts of all compositions of n with at least two parts
 *          in increasing order.
 * https://oeis.org/A229936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2^(n-1) – P(n)) * where P(n) is the partition function
 *            A000041(n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, free; for C++ std::free is acceptable */
#if defined(__cplusplus)
#include <new>                      /* for 'new' and 'delete' */
#endif
#include "../utils/big_pow.h"       /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 no compositions with at least two parts exist. */
    if (n <= 2)
        return 0;

    /* ---------- Compute partition numbers P[0..n] using Euler’s recurrence ----------
     * P(0) = 1. For m >= 1:
     *   P(m) = Σ_{k=1..∞} (-1)^(k-1) P(m - p_k) + (-1)^(k-1) P(m - q_k)
     *   where p_k = k*(3k-1)/2 and q_k = k*(3k+1)/2.
     * We accumulate positive and negative contributions separately to avoid
     * unsigned underflow, then perform a single subtraction, which is safe
     * because the final result is non‑negative.
     */

    /* Allocate an array of partition values.  Use C++ new if available. */
#if defined(__cplusplus)
    seq_t *P = new seq_t[n + 1];
#else
    seq_t *P = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!P)
        return 0; /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        P[i] = 0;
    P[0] = 1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = (k * (3 * k - 1)) / 2;
            if (pent1 > m)
                break;
            if (k & 1)
                pos += P[m - pent1];
            else
                neg += P[m - pent1];

            idx_t pent2 = (k * (3 * k + 1)) / 2;
            if (pent2 > m)
                continue;
            if (k & 1)
                pos += P[m - pent2];
            else
                neg += P[m - pent2];
        }
        P[m] = pos - neg;  /* guaranteed non‑negative */
    }

    /* ---------- Compute 2^(n-1) using the provided big_pow function ----------
     * pow2 = 2^(n-1)
     */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 1));

    /* Differences and final multiplication */
    seq_t diff = pow2 - P[n];
    seq_t result = ((seq_t)n) * diff;

    /* Free the allocated memory */
#if defined(__cplusplus)
    delete [] P;
#else
    free(P);
#endif

    return result;
}

/*
 * A026804  Number of partitions of n in which the least part is odd.
 * https://oeis.org/A026804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{odd k ≥ 1} p(n−k, k) where p(m, k) = number of
 *            partitions of m into parts ≥ k. The algorithm builds p(·, k) in
 *            place using a one‑dimensional DP and accumulates the answer for
 *            odd k
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 128, u64 n <= 417,
 *            u128 n <= 1459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static storage for the DP table and the result array.
   They are lazily allocated and resized on demand. */
static seq_t *dp   = NULL;   /* dp[i] = number of partitions of i with parts ≥ current k */
static seq_t *A    = NULL;   /* A[n] = a(n) for 0 ≤ n ≤ Nmax */
static idx_t   Nmax = 0;     /* largest n processed so far */

/* Allocate and compute values up to N (inclusive). Frees old arrays. */
static void compute_up_to(idx_t N)
{
    /* free previous arrays */
    if (dp) free((void *)dp);
    if (A)  free((void *)A);

    /* allocate zeroed arrays */
    dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    A  = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));

    if (!dp || !A) return;      /* allocation failed – nothing to do */

    dp[0] = (seq_t)1;           /* p(0, k) = 1 for any k */

    /* Process part sizes from N down to 1.
       After the inner loop, dp[i] holds p(i, k). */
    for (idx_t k = N; k > 0; k--) {
        for (idx_t i = k; i <= N; i++) {
            dp[(size_t)i] += dp[(size_t)(i - k)];
        }
        if (k & 1) {                 /* odd k contributes to the answer */
            for (idx_t i = k; i <= N; i++) {
                A[(size_t)i] += dp[(size_t)(i - k)];
            }
        }
    }
    Nmax = N;
}

/* Return a(n) for n ≥ 1.  The function is safe to call in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0   */
    if ((idx_t)n > Nmax) compute_up_to(n);
    return A[(size_t)n];
}

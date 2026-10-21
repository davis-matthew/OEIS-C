/*
 * A205032  a(n) = (s(k)-s(j))/n, where (s(k),s(j)) is the least pair of
 *          oblong numbers (A002378) for which n divides their difference;
 *          a(n) = (1/n)*A205031(n).
 * https://oeis.org/A205032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (s(k)-s(j))/n, where s(m) = m(m+1) is an oblong number;
 *            the pair (k,j) is the first (smallest indices) pair with n |
 *            (s(k)-s(j))
 * Range:     u8 n <= 2544, u16 n <= 9196, u32 n <= 9196, u64 n <= 9196,
 *            u128 n <= 9196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;   /* a(1) = 4 */

    /* Allocate an array to store the first occurrence of each residue mod n. */
    size_t N = (size_t)n;                 /* n fits in size_t for n <= 10000 */
    idx_t *first = (idx_t *)malloc(N * sizeof(idx_t));
    if (!first) return (seq_t)0;          /* allocation failure */

    for (size_t i = 0; i < N; ++i) first[i] = 0;

    /* s(k) = k(k+1).  Start with k = 1. */
    idx_t mod = 2 % n;          /* s(1) = 1*2 = 2 */
    idx_t k = 1;
    first[mod] = k;             /* record first occurrence of this residue */

    /* Search k = 2 .. n+1 for the first repeated residue. */
    for (k = 2; k <= N + 1; ++k) {
        /* s(k) = s(k-1) + 2k  (since k(k+1) - (k-1)k = 2k) */
        mod = (mod + (2 * k) % n) % n;    /* keep value in 0..n-1 */

        if (first[mod] == 0) {
            first[mod] = k;               /* first occurrence */
        } else {
            idx_t j = first[mod];         /* matching earlier index */

            /* Compute difference d = s(k) - s(j) = (k-j)(k+j+1) */
            idx_t m = k - j;
            idx_t t = k + j + 1;
            seq_t d = (seq_t)m * (seq_t)t;

            seq_t result = d / (seq_t)n;  /* a(n) = d / n */

            free(first);
            return result;
        }
    }

    /* In theory we should never reach here for n >= 2. */
    free(first);
    return (seq_t)0;
}

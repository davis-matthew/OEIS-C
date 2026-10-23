/*
 * A143127  a(n) = Sum_{k=1..n} k*d(k) where d(k) is the number of divisors of
 *          k.
 * https://oeis.org/A143127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k * d(k) where d(k) = number of divisors of
 *            k
 * Range:     u8 n <= 11, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t maxN = 0;          /* maximum n for which we have computed values */
static seq_t *A = NULL;         /* cumulative sums a(k) for k = 0 .. N */
static idx_t *d = NULL;         /* divisor counts for 0 .. N */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* defined for safety */

    /* If we haven't computed up to this n, extend the tables */
    if (n > maxN) {
        /* Allocate space for indices 0..n (inclusive) */
        if (A == NULL) {
            A = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        } else {
            A = (seq_t *)realloc(A, ((size_t)n + 1) * sizeof(seq_t));
        }
        if (A == NULL) exit(1);               /* allocation failed */

        if (d == NULL) {
            d = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
        } else {
            d = (idx_t *)realloc(d, ((size_t)n + 1) * sizeof(idx_t));
        }
        if (d == NULL) exit(1);               /* allocation failed */

        /* Zero divisor counts */
        for (idx_t i = 1; i <= n; ++i) d[i] = 0;

        /* Sieve to compute divisor counts */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++d[j];
            }
        }

        /* Compute cumulative sums */
        A[0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            A[k] = A[k-1] + (seq_t)k * (seq_t)d[k];
        }

        maxN = n;
    }

    return A[n];
}

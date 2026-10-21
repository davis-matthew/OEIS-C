/*
 * A051950  Differences between values of tau(n) (A000005): a(n) =
 *          tau(n)-tau(n-1).
 * https://oeis.org/A051950
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = tau(n) - tau(n-1), n ≥ 2 where tau(n) = number of
 *            positive divisors of n
 * Range:     i8 n <= 7623, i16 n <= 7623, i32 n <= 7623, i64 n <= 7623,
 *            i128 n <= 7623, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#ifdef __cplusplus
    #include <new>
    static seq_t *alloc_seq(size_t sz) { return new seq_t[sz]; }
    static void   free_seq(seq_t *p)   { delete[] p; }
#else
    #include <stdlib.h>  /* malloc, free */
    static seq_t *alloc_seq(size_t sz) { return (seq_t *)malloc(sz * sizeof(seq_t)); }
    static void   free_seq(seq_t *p)   { free(p); }
#endif

extern seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For smaller n we simply
       return 0 – the caller never uses the value. */
    if (n < 2) return 0;

    /* Static cache holding tau[1..max_cached].  tau[0] is unused. */
    static idx_t   max_cached = 1;   /* at least 1 since tau[1]=1 */
    static seq_t * tau       = NULL;

    /* If the requested n is beyond the cache, recompute from scratch. */
    if (n > max_cached) {
        /* Allocate a new array large enough for indices 0..n. */
        seq_t *new_tau = alloc_seq(n + 1);
        if (!new_tau) return 0;  /* allocation failed – unlikely */

        /* Initialise all entries to zero. */
        for (idx_t i = 0; i <= n; ++i) new_tau[i] = 0;

        /* Compute divisor counts via a simple sieve.  tau[1] = 1. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                new_tau[j] += 1;
            }
        }

        /* Free old cache and replace it. */
        free_seq(tau);
        tau = new_tau;
        max_cached = n;
    }

    /* Return the difference tau(n) - tau(n‑1). */
    return tau[n] - tau[n-1];
}

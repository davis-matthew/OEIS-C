/*
 * A208251  Number of refactorable numbers less than or equal to n.
 * https://oeis.org/A208251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{m <= n : m is refactorable (m % d(m) == 0)}
 * Range:     u8 n <= 3279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes divisor counts up to the largest n requested
   and stores the prefix sums of refactorable numbers. Subsequent calls reuse
   the cached data. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be at least 1 for valid terms; n==0 returns 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Static cache variables: divisor counts and prefix sums of refactorable
       numbers. They are grown as needed. */
    static uint32_t *div_counts = NULL;   /* divisor counts, indices 1..N */
    static seq_t   *pref_counts = NULL;   /* prefix sums, indices 0..N */
    static idx_t    Nmax = 0;              /* maximum n cached so far */

    if (n > Nmax) {
        /* Allocate new arrays of size n+1 (indices 0..n). */
        div_counts = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
        pref_counts = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!div_counts || !pref_counts) {
            /* In a production program we would report the error; here we
               simply return 0 to satisfy the interface. */
            return (seq_t)0;
        }

        /* Compute divisor counts for 1..n by a simple sieve. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++div_counts[j];
            }
        }

        /* Compute prefix sums of refactorable numbers. */
        seq_t cnt = 0;
        for (idx_t i = 1; i <= n; ++i) {
            if (i % div_counts[i] == 0) {
                ++cnt;
            }
            pref_counts[i] = cnt;
        }

        Nmax = n;
    }

    return pref_counts[n];
}

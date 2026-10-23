/*
 * A136548  a(n) = max {k >= 1 | sigma(k) <= n}.
 * https://oeis.org/A136548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{k >= 1 | sigma(k) <= n} computed by a sieve over
 *            divisors. The function recomputes sigma up to n when necessary
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* External function signature required by the build system */
seq_t a(idx_t n)
{
    /* convert n to seq_t for comparisons */
    seq_t nval = (seq_t)n;

    /* global cache of sigma values, expanded as needed */
#if defined(__cplusplus)
    /* In C++ use a std::vector so that elements are properly
       constructed even if seq_t is a big integer class. */
    static std::vector<seq_t> sigma;
    static idx_t cached_n = 0;          /* largest n for which we have data */
    /* resize if needed, and reset all entries to 0 */
    if (n > cached_n) {
        sigma.assign(n + 1, 0);          /* assigns n+1 zeros */
        cached_n = n;
    } else {
        std::fill(sigma.begin(), sigma.begin() + n + 1, 0);
    }
    /* Compute sigma by the classic divisor-sum sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }
    /* Find the largest k with sigma(k) <= n */
    seq_t best = 1; /* a(1)=1 always satisfies the condition */
    for (idx_t k = 2; k <= n; ++k) {
        if (sigma[k] <= nval) {
            best = (seq_t)k;
        }
    }
    return best;
#else
    /* In C use a malloc'ed array of seq_t elements. */
    static seq_t *sigma = NULL;
    static idx_t cached_n = 0;      /* largest n for which we have data */
    static size_t cache_cap = 0;    /* allocated size */

    /* Resize if needed */
    if (n > cached_n) {
        size_t new_cap = n + 1;
        seq_t *tmp = realloc(sigma, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* out of memory */
        }
        sigma = tmp;
        cache_cap = new_cap;
        cached_n = n;
    }
    /* Zero all entries up to n */
    memset(sigma, 0, (n + 1) * sizeof(seq_t));

    /* Compute sigma via the divisor-sum sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* Find the largest k with sigma(k) <= n */
    seq_t best = 1; /* a(1)=1 always satisfies the condition */
    for (idx_t k = 2; k <= n; ++k) {
        if (sigma[k] <= nval) {
            best = (seq_t)k;
        }
    }
    return best;
#endif
}

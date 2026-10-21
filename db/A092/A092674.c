/*
 * A092674  Derived from a(n)=binomial(n+1,2) - sum{i=1,n-1,a(i)*floor(n/i)}
 *          (see A000010) - this is the value of the constant term.
 * https://oeis.org/A092674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+1,2) - \sum_{i=1}^{n-1} a(i) * floor(n/i)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache storage for a(n) values */
static seq_t *a_cache   = NULL; /* array, 1‑based indices: a_cache[n] = a(n) */
static size_t   cache_cap = 0;  /* allocated number of elements (>= n+1) */
static idx_t    max_cached = 0; /* highest n that has been computed */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while ((size_t)n + 1 > new_cap)
        new_cap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_a)
        abort(); /* out of memory */

    /* Initialise new slots to zero (they will be written soon). */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_a[i] = 0;

    a_cache = new_a;
    cache_cap = new_cap;
}

/* Computes a(n) for n >= 1, memoising results. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but safe */

    ensure_cache(n);

    /* Initialise a(1) if not already done. */
    if (max_cached < 1) {
        a_cache[1] = 0; /* a(1) = 0 */
        max_cached = 1;
    }

    if (n > max_cached) {
        for (idx_t m = max_cached + 1; m <= n; ++m) {
            seq_t val;
            if (m == 1) {           /* handled above, but keep for clarity */
                val = 0;
            } else {
                /* binomial(m+1,2) = m*(m+1)/2 */
                seq_t binom = ((seq_t)m * ((seq_t)m + 1)) / 2;

                /* Compute the sum part. */
#if defined(__SIZEOF_INT128__)
                unsigned __int128 sum128 = 0;
                for (idx_t j = 1; j < m; ++j) {
                    unsigned __int128 prod =
                        (unsigned __int128)a_cache[j] *
                        (unsigned __int128)(m / j);
                    sum128 += prod;
                }
                val = binom - (seq_t)sum128;
#else
                seq_t sum = 0;
                for (idx_t j = 1; j < m; ++j) {
                    sum += a_cache[j] * (seq_t)(m / j);
                }
                val = binom - sum;
#endif
            }
            a_cache[m] = val;
        }
        max_cached = n;
    }

    return a_cache[n];
}

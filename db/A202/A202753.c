/*
 * A202753  Number of n X 5 nonnegative integer arrays with each row and
 *          column increasing from zero by 0 or 1.
 * https://oeis.org/A202753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) +
 *            462*a(n-5) * - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) + 55*a(n-9)
 *            - 11*a(n-10) + a(n-11) * Initial values (n=1..11): 1, 5, 31,
 *            184, 924, 3809, 13197, 39675, 106357, * 259669, 586829
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache   = NULL;   /* cache[0] unused, cache[1] == a(1) */
static size_t cache_cap = 0;    /* allocated capacity (elements)   */
static size_t cache_len = 0;    /* largest n for which a(n) is stored */

/* Ensure that a(n) is computed and stored in cache[n] (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    /* Grow cache array to at least (n+1) elements, plus a safety margin. */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        while (n + 1 > new_cap)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base values if not yet done. */
    static const seq_t base_vals[12] = {
        0,                 /* dummy for index 0 */
        1ULL, 5ULL, 31ULL, 184ULL, 924ULL,
        3809ULL, 13197ULL, 39675ULL, 106357ULL,
        259669ULL, 586829ULL
    };

    if (cache_len < 11) {
        /* Copy base values up to index 11. */
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = base_vals[i];
        cache_len = 11;
    }

    /* If the requested n is within already computed range, nothing more to do. */
    if ((idx_t)cache_len >= n)
        return;

    /* Recurrence coefficients (index 1..11). */
    static const int coeff[12] = {
        0,  /* unused 0 */
        11, -55, 165, -330, 462, -462,
        330, -165, 55, -11, 1
    };

    /* Compute new terms from (cache_len + 1) up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        __int128 sum = 0;
        for (int j = 1; j <= 11; ++j) {
            sum += (__int128)cache[i - j] * coeff[j];
        }
        cache[i] = (seq_t)sum;
    }

    cache_len = (size_t)n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    ensure_cache(n);
    return cache[(size_t)n];
}

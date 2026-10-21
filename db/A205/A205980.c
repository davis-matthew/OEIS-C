/*
 * A205980  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + 26*a(n-3) + 5*a(n-4) * - 12*a(n-6) -
 *            4*a(n-7) for n > 9 * Initial terms (n = 1 … 9): * 423, 1980,
 *            8004, 33504, 140802, 591336, 2482596, 10425528, 43778766
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_terms[9] = {
    423ULL, 1980ULL, 8004ULL,
    33504ULL, 140802ULL, 591336ULL,
    2482596ULL, 10425528ULL, 43778766ULL
};

static seq_t *cache = NULL;      /* storage for a(1) .. a(cache_max)   */
static size_t cache_cap = 0;     /* number of elements allocated       */
static idx_t cache_max = 0;      /* highest index currently stored (1‑based) */

/* Allocate or grow the cache so that it can hold at least n elements
   (indices 0 … n-1).  The cast is necessary for C++ validity. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *newc = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!newc)
        abort();                      /* memory allocation failure */
    cache = newc;
    cache_cap = new_cap;
}

/* If the first nine terms are not yet stored, store them in the cache. */
static void fill_base(void)
{
    if (cache_max >= 9)
        return;
    ensure_cache(9);                /* guarantee space for 9 elements */
    for (idx_t i = cache_max + 1; i <= 9; ++i)
        cache[i - 1] = base_terms[i - 1];
    cache_max = 9;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n = 0, return 0 */
    if (n <= 9)
        return base_terms[n - 1];

    /* Make sure the base terms are present in the cache. */
    fill_base();

    /* Ensure capacity for all terms up to n. */
    ensure_cache(n);

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* 1-based indices: a(i-1) -> cache[i-2], etc. */
        seq_t v1 = cache[i - 2];     /* a(i-1) */
        seq_t v2 = cache[i - 3];     /* a(i-2) */
        seq_t v3 = cache[i - 4];     /* a(i-3) */
        seq_t v4 = cache[i - 5];     /* a(i-4) */
        seq_t v6 = cache[i - 7];     /* a(i-6) */
        seq_t v7 = cache[i - 8];     /* a(i-7) */

        seq_t term_pos = v1 + 7 * v2 + 26 * v3 + 5 * v4;
        seq_t term_neg = 12 * v6 + 4 * v7;
        cache[i - 1] = term_pos - term_neg;
    }

    cache_max = n;
    return cache[n - 1];
}

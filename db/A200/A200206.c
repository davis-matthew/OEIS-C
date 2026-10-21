/*
 * A200206  Number of -n..n arrays x(0..4) of 5 elements with zero sum and
 *          nonzero second and third differences.
 * https://oeis.org/A200206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-5)+a(n-6)-2*a(n-7)+2*a(n-8)-2*a(n-9)+
 *            2*a(n-13)-2*a(n-14)+2*a(n-15)-a(n-16)+a(n-17)-a(n-19)
 *            -a(n-21)+a(n-22). Initial terms (a(1)..a(22)) = 22, 246, 1106,
 *            3196, 7450, 14842, 26932, 44910, 71050, 106802, 155056, 217506,
 *            297896, 397820, 522032, 672214, 853824, 1068640, 1323...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 145, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[23] = {
    0ULL, /* unused index 0 */
    22ULL, 246ULL, 1106ULL, 3196ULL, 7450ULL, 14842ULL,
    26932ULL, 44910ULL, 71050ULL, 106802ULL, 155056ULL,
    217506ULL, 297896ULL, 397820ULL, 522032ULL, 672214ULL,
    853824ULL, 1068640ULL, 1323238ULL, 1619004ULL, 1963760ULL,
    2358796ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that the cache can hold indices up to n (1‑based).  */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* space for indices 0..n */
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the OEIS A200206 term a(n).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1; unused <1 values */

    /* For the first 22 terms return the hard‑coded values.  */
    if (n <= 22) return init_vals[(size_t)n];

    /* Ensure the cache can hold the required indices. */
    ensure_cache_size(n);

    /* Copy the initial values into the cache (irrelevant if already set). */
    for (size_t i = 1; i <= 22; ++i) cache[i] = init_vals[i];

    /* Compute terms from 23 up to n, using the empirical recurrence.  */
    for (size_t i = 23; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] + cache[i-3] - cache[i-5] + cache[i-6]
                  - (seq_t)2 * cache[i-7]
                  + (seq_t)2 * cache[i-8]
                  - (seq_t)2 * cache[i-9]
                  + (seq_t)2 * cache[i-13]
                  - (seq_t)2 * cache[i-14]
                  + (seq_t)2 * cache[i-15]
                  - cache[i-16] + cache[i-17]
                  - cache[i-19] - cache[i-21] + cache[i-22];
        cache[i] = val;
    }

    return cache[(size_t)n];
}

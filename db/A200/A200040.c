/*
 * A200040  Number of -n..n arrays x(0..3) of 4 elements with sum zero and
 *          with zeroth through 3rd differences all nonzero.
 * https://oeis.org/A200040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-5) - a(n-6) - a(n-7) - a(n-8) +
 *            2*a(n-10) + a(n-11) - a(n-13) (n ≥ 14) initial values
 *            a(1)..a(13) are as listed above
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- initial values --------------------------------------------------- */
static const seq_t initial[14] = {
    0ULL,          /* 0 unused – offset starts at 1 */
    2ULL, 28ULL, 84ULL, 218ULL, 444ULL, 808ULL,
    1330ULL, 2068ULL, 2996ULL, 4230ULL, 5740ULL,
    7552ULL, 9746ULL
};

/* --- memoisation storage ---------------------------------------------- */
static seq_t *cache      = NULL;  /* cache[0] unused */
static idx_t  cache_size = 0;     /* allocated size, >=1 when cache!=NULL */
static idx_t  computed   = 0;     /* highest index computed so far */

/* --- ensure cache is large enough ------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;               /* nothing requested */
    idx_t new_size = n + 1;           /* we need indices 0..n */
    if (cache_size >= new_size) return;

    seq_t *tmp = (cache == NULL) ? NULL : (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (cache == NULL) tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (tmp == NULL) {                /* allocation failure – abort silently */
        return;
    }
    /* initialize any newly allocated slot */
    for (idx_t i = cache_size; i < new_size; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_size = new_size;
}

/* --- main sequence routine -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    /* return the known initial value if requested */
    if (n <= 13) return initial[(size_t)n];

    /* ensure the cache is large enough and copy initial values if the
       cache is being created for the first time */
    if (cache == NULL) {
        ensure_cache(n);
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = initial[i];
        computed = 13;
    } else {
        ensure_cache(n);
    }

    /* compute only the missing values */
    idx_t start = computed + 1;
    if (start < 14) start = 14;
    for (idx_t i = start; i <= n; ++i) {
        __int128 val =
            (__int128)cache[i - 2]
          + 2 * (__int128)cache[i - 3]
          - (__int128)cache[i - 5]
          - (__int128)cache[i - 6]
          - (__int128)cache[i - 7]
          - (__int128)cache[i - 8]
          + 2 * (__int128)cache[i - 10]
          + (__int128)cache[i - 11]
          - (__int128)cache[i - 13];
        cache[i] = (seq_t)val;
    }
    computed = n;
    return cache[n];
}

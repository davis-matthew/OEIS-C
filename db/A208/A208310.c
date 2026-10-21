/*
 * A208310  Number of n X 4 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 21*a(n-2) + 21*a(n-3) - 9*a(n-4) (n>=5)
 *            Initial values: a(1)=8, a(2)=54, a(3)=336, a(4)=2052 The
 *            function memoizes computed values so that subsequent calls are
 *            O(1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoisation storage */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms currently stored */

/* initialise cache with first four terms */
static void init_cache(void)
{
    if (cache_len != 0) return;          /* already initialised */

    const seq_t first[4] = {8, 54, 336, 2052};

    /* allocate capacity for at least 4 terms */
    cache_cap = 4;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < 4; ++i) cache[i] = first[i];
    cache_len = 4;
}

/* ensure capacity for at least 'need' terms */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, return 0 for safety */

    init_cache();                   /* ensure first four terms are stored */

    if (n <= cache_len) return cache[n - 1];

    /* compute values from cache_len+1 up to n using the recurrence */
    ensure_cap((size_t)n);
    for (size_t i = cache_len; i < n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        seq_t s1 = a1 * 9;                /* 9*a(n-1) */
        seq_t s2 = s1 - a2 * 21;           /* -21*a(n-2), safe: s1 >= a2*21 */
        seq_t s3 = s2 + a3 * 21;           /* +21*a(n-3) */
        seq_t val = s3 - a4 * 9;           /* -9*a(n-4), safe: s3 >= a4*9 */

        cache[i] = val;
    }
    cache_len = n;
    return cache[n - 1];
}

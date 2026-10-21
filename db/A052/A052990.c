/*
 * A052990  Expansion of ( 1-x ) / ( 1-4*x-x^2+2*x^3 ).
 * https://oeis.org/A052990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=13, a(n)=4*a(n-1)+a(n-2)-2*a(n-3) for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052990 – OEIS recurrence (forward form)          */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;     /* cached terms      */
static size_t cache_len = 0;    /* allocated length  */
static size_t computed_max = 0; /* highest index computed so far */

/* Ensure cache is large enough for indices 0 … n */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_len < needed) {
        size_t new_len = cache_len * 2;
        if (new_len < needed) new_len = needed;
        if (new_len < 3)      new_len = 3;          /* at least 3 for initial values */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* realloc may fail, but we ignore that case for contest code */
        cache = tmp;
        cache_len = new_len;
    }
}

/* Initialise the first three terms if not already done */
static void init_terms(void)
{
    if (computed_max < 2) {
        /* make sure we have space for indices 0,1,2 */
        ensure_cache_size(2);
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 13;
        computed_max = 2;          /* 0,1,2 are now computed */
    }
}

/* Public function */
seq_t a(idx_t n)
{
    /* quick check for very small n to avoid loops */
    if (n <= 2) {
        init_terms();
        return cache[(size_t)n];
    }

    ensure_cache_size(n);
    init_terms();

    /* compute any missing terms up to n */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        cache[i] = (4 * cache[i - 1]) + cache[i - 2] - (2 * cache[i - 3]);
    }
    computed_max = (size_t)n;

    return cache[(size_t)n];
}

/*
 * A194273  Concentric triangular numbers (see Comments lines for definition).
 * https://oeis.org/A194273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194273 – Concentric triangular numbers
   Defined by the linear recurrence
        a(0)=0 , a(1)=1 , a(2)=3 , a(3)=6 , a(4)=9 , a(5)=12 , a(6)=15
        a(n)=3*a(n-1)-3*a(n-2)+3*a(n-4)-3*a(n-5)+a(n-6)   for n≥6
   (generating function x/(1-3x+3x^2-3x^4+3x^5-x^6) )
   The function below memoises the values to answer any call in O(1) after
   the needed terms have been computed. */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache   = NULL;    /* memoised terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms that have been
                                   computed and stored in cache */

/* initialise cache with the first seven terms */
static void init_cache(void)
{
    if (cache != NULL) return;

    /* start with a modest capacity – will grow as needed */
    cache_cap = 32;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) return;   /* allocation failure – function will crash
                            later, matching the build environment */

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)9;
    cache[5] = (seq_t)12;
    cache[6] = (seq_t)15;
    cache_len = 7;        /* terms 0 … 6 are now valid */
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_cache();

    /* return the value if it has already been computed */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow the array if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < required)
            new_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)                 /* allocation failure – abort */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = 3 * cache[i - 1] - 3 * cache[i - 2];
        seq_t t2 = 3 * cache[i - 4] - 3 * cache[i - 5];
        cache[i] = t1 + t2 + cache[i - 6];
    }
    cache_len = required;     /* all terms < required are now valid */

    return cache[(size_t)n];
}

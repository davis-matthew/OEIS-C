/*
 * A063278  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          5 ).
 * https://oeis.org/A063278
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13. Initial values (n ≥
 *            2): a(2) = -1, a(3)=0, a(4)=0, a(5)=1, a(6)=2, a(7)=3, a(8)=4,
 *            a(9)=5, a(10)=6, a(11)=7, a(12)=8, a(13)=7
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* computed values */
static idx_t  cache_size = 0;      /* allocated size of cache */
static idx_t  computed_upto = 0;   /* highest n with a(n) present */

/* enlarge cache so that cache[n] is valid to write */
static void grow_cache(idx_t n)
{
    if (cache_size > n)
        return;
    idx_t new_size = cache_size ? cache_size : 64;
    while (new_size <= n)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

/* initialise known values up to n = 13 */
static void init_base(void)
{
    if (cache != NULL)
        return;
    cache_size = 64;
    cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    if (cache == NULL)
        return;                       /* out of memory – callers will fail gracefully */

    cache[2]  = (seq_t)-1;
    cache[3]  = (seq_t)0;
    cache[4]  = (seq_t)0;
    cache[5]  = (seq_t)1;
    cache[6]  = (seq_t)2;
    cache[7]  = (seq_t)3;
    cache[8]  = (seq_t)4;
    cache[9]  = (seq_t)5;
    cache[10] = (seq_t)6;
    cache[11] = (seq_t)7;
    cache[12] = (seq_t)8;
    cache[13] = (seq_t)7;
    computed_upto = 13;
}

/* compute a(i) for all i up to n using the recurrence */
static void compute_to(idx_t n)
{
    if (n < 2)
        return;
    if (computed_upto >= n)
        return;
    grow_cache(n);
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i > 13) {                       /* recurrence applies */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        } /* indices ≤ 13 are already initialised */
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    init_base();
    if (n < 2)                         /* undefined region – return 0 */
        return (seq_t)0;
    compute_to(n);
    return cache[n];
}

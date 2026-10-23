/*
 * A180669  a(n) = a(n-1)+a(n-2)+a(n-3)+4*n^2-16*n+18 with a(0)=0, a(1)=0 and
 *          a(2)=1.
 * https://oeis.org/A180669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+4*n^2-16*n+18 * Initial values:
 *            a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 69, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised cache */
static seq_t *cache = NULL;        /* array holding a(0)…a(max) */
static size_t cache_cap = 0;       /* allocated size of cache */
static idx_t  computed_max = 0;    /* largest n that has been computed */

/* Ensure room for indices up to n (inclusive).  Doubles capacity until large
 * enough.  Newly allocated slots are left uninitialised except for the
 * obligatory base values a(0), a(1) and a(2). */
static void grow_cache(idx_t n)
{
    if (cache_cap > n)
        return;                               /* already large enough */

    size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                        /* unrecoverable memory failure */

    cache = tmp;

    /* On the very first allocation set the three base terms.  They are
     * always needed soon after a grow. */
    if (cache_cap == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        if (new_cap > 2)
            cache[2] = (seq_t)1;
    }

    cache_cap = new_cap;

    /* If we just created storage and it is large enough for the third
     * base value, mark it as computed. */
    if (computed_max < 2 && cache_cap > 2)
        computed_max = 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    grow_cache(n);

    /* If the requested value has already been computed, return it. */
    if (computed_max >= n)
        return cache[n];

    /* Compute sequentially all missing terms up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term  = (seq_t)4 * i_seq * i_seq - (seq_t)16 * i_seq + (seq_t)18;
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + term;
    }
    computed_max = n;
    return cache[n];
}

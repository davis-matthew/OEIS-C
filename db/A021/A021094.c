/*
 * A021094  Expansion of 1/((1-x)*(1-2*x)*(1-4*x)*(1-11*x)).
 * https://oeis.org/A021094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 18*a(n-1) - 91*a(n-2) + 162*a(n-3) - 88*a(n-4)
 * for n >= 4
 * a(0)=1, a(1)=18, a(2)=233, a(3)=2718
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed terms */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* ensure the cache can store terms up to n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n + 1) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;

    /* initialise the first four terms on the first allocation */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)233;
        cache[3] = (seq_t)2718;
        cache_len = 4;
    }
}

seq_t a(idx_t n)
{
    /* quick return for the first four terms */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)18;
    if (n == (idx_t)2) return (seq_t)233;
    if (n == (idx_t)3) return (seq_t)2718;

    ensure_cache(n);

    /* compute missing terms on demand */
    while (cache_len <= (size_t)n) {
        size_t k = cache_len;          /* index of term to compute */
        seq_t term = (seq_t)18 * cache[k - 1];
        term -= (seq_t)91 * cache[k - 2];
        term += (seq_t)162 * cache[k - 3];
        term -= (seq_t)88 * cache[k - 4];
        cache[cache_len++] = term;
    }

    return cache[n];
}

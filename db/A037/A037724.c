/*
 * A037724  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal value of the base‑5 number whose first n
 *            digits are the repeating period 2,0,3,1 (started with 2). The
 *            computation is performed iteratively: a(1)=2, a(n)=a(n-1)*5 + d,
 *            where d is the next digit in the cycle
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;     /* cached values a(0..max_n), index 0 unused */
static idx_t  cache_max = 0;    /* largest n cached so far */
static size_t cache_cap = 0;    /* current allocation size */

static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    /* grow capacity to at least n+1 (index 0 unused) */
    size_t target = (size_t)n + 1;
    if (cache_cap == 0) cache_cap = 1;
    while (cache_cap < target) cache_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (new_cache == NULL) abort();   /* OOM, unlikely in tests */
    cache = new_cache;

    /* start from the previously cached last value (or 0 if none) */
    seq_t val = (cache_max ? cache[cache_max] : 0);
    idx_t i;
    for (i = cache_max + 1; i <= n; ++i)
    {
        unsigned digit;
        switch ((i - 1) & 3u)   /* digit cycle 2,0,3,1 */
        {
            case 0: digit = 2; break;
            case 1: digit = 0; break;
            case 2: digit = 3; break;
            case 3: digit = 1; break;
        }
        val = val * 5 + digit;
        cache[i] = val;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n<1, but return 0 */
    ensure_cache(n);
    return cache[n];
}

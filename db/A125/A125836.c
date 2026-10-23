/*
 * A125836  Numbers whose base 8 or octal representation is 555555555......5.
 * https://oeis.org/A125836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*(8^(n-1)-1)/7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc / realloc */

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array indexed from 1 */
static idx_t cache_len = 0;          /* highest computed index */
static seq_t pow8_cur = (seq_t)1;    /* 8^(cache_len-1) */

/* ------------------------------------------------------------ */
/*  OEIS A125836  Numbers whose base 8 representation is 555…5   */
seq_t a(idx_t n)
{
    if (n < 1) /* undefined, return 0 as safe default */
        return (seq_t)0;

    /* already computed? */
    if (cache_len >= n)
        return cache[n];

    /* enlarge cache if needed */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
    if (!new_cache) {                 /* allocation failed – compute directly */
        seq_t pow8 = (seq_t)1;
        for (idx_t i = 1; i < n; ++i)
            pow8 *= (seq_t)8;
        seq_t t = pow8 - (seq_t)1;
        t /= (seq_t)7;
        return t * (seq_t)5;
    }
    cache = new_cache;

    /* initialise cache if first use */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;      /* a(1) = 0 */
        cache_len = 1;
        pow8_cur = (seq_t)1;      /* 8^(1-1) = 1 */
    }

    /* compute missing terms extending from current maximum */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        pow8_cur *= (seq_t)8;                     /* 8^(i-1)               */
        seq_t t = pow8_cur - (seq_t)1;            /* 8^(i-1) - 1           */
        t /= (seq_t)7;                            /* (8^(i-1)-1)/7          */
        cache[i] = t * (seq_t)5;                  /* 5 * (...)               */
    }
    cache_len = n;

    return cache[n];
}

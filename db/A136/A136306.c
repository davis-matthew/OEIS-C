/*
 * A136306  a(n) = a(n-1)*(10^K) + n*a(n-1); a(0)=1; K=floor(log_10
 *          (n*a(n-1))).
 * https://oeis.org/A136306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=a(n-1)*(10^K)+n*a(n-1), K=floor(log10(n*a(n-1)))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  A136306 ------------- */

/* -------------  helpers ------------- */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* -------------  cache ------------- */
static seq_t *cache     = NULL;   /* array of computed terms               */
static idx_t  cache_size  = 0;    /* current allocation size              */
static idx_t  computed_max = 0;   /* highest index that has been computed */

/* -------------  resize cache ------------- */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                                   /* already big enough */

    idx_t new_size = cache_size ? cache_size : 1;
    while (new_size <= n)
        new_size <<= 1;                            /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

/* -------------  sequence a(n) ------------- */
seq_t a(idx_t n)
{
    /* make sure array is big enough */
    ensure_cache(n);

    /* initialise the first term if needed */
    if (computed_max == 0) {
        cache[0] = (seq_t)1;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t prev   = cache[i-1];                 /* a(i-1)   */
        seq_t prod   = (seq_t)i * prev;            /* i * a(i-1) */
        int   digits = digit_len(prod);            /* decimal digits of i*a(i-1) */
        idx_t K = (idx_t)(digits - 1);             /* K = floor(log10(prod)) */

        seq_t pow10k = pow10(K);                   /* 10^K          */
        seq_t factor = pow10k + (seq_t)i;          /* 10^K + i      */
        cache[i] = prev * factor;                  /* a(i)          */
    }

    if (n > computed_max)
        computed_max = n;

    return cache[n];
}

/*
 * A237861  Number of (n+1)X(1+1) 0..2 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=81,a(2)=336,a(3)=1414,a(4)=6030,a(5)=25525,a(6)=107272,a(7)=453088,a(8)=1919632,
 *            a(9)=8117752,a(10)=34231344,a(11)=144659936,a(12)=611705384,a(13)=2586040156,a(14)=10917301088;
 *            For n ≥ 15 the linear recurrence holds:
 *            a(n)=252*a(n‑4)+1304*a(n‑6)-1908*a(n‑8)-1312*a(n‑10)+1280*a(n‑12)-2304*a(n‑14)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* liveness of cache */
static seq_t *cache = NULL;   /* 0‑based array, cache[i] = a(i+1) */
static size_t cache_len = 0;  /* number of terms stored (always ≤ cache_cap) */
static size_t cache_cap = 0;  /* capacity of cache array */

/* initial 14 terms */
static const seq_t init[14] = {
    (seq_t)81,          (seq_t)336,          (seq_t)1414,          (seq_t)6030,
    (seq_t)25525,       (seq_t)107272,       (seq_t)453088,        (seq_t)1919632,
    (seq_t)8117752,     (seq_t)34231344,     (seq_t)144659936,     (seq_t)611705384,
    (seq_t)2586040156,  (seq_t)10917301088
};

/* initialise cache with the first 14 terms */
static void init_cache(void)
{
    if (cache != NULL) return;                /* already initialised */
    cache_cap = 14;
    cache_len = 14;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i) cache[i] = init[i];
}

/* enlarge cache if needed to hold up to 'needed' terms */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL) {                      /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = needed;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* defined only for n ≥ 1 */

    if (cache_len == 0) init_cache();        /* lazy initialisation */

    size_t k = (size_t)n;                 /* desired number of terms */
    ensure_capacity(k);

    /* compute missing terms */
    if (k > cache_len) {
        for (size_t i = cache_len; i < k; ++i) {
            /* index i corresponds to term a(i+1) */
            seq_t val = 252 * cache[i-4]   /* a(n-4) */
                      + 1304 * cache[i-6]  /* a(n-6) */
                      - 1908 * cache[i-8]  /* a(n-8) */
                      - 1312 * cache[i-10] /* a(n-10) */
                      + 1280 * cache[i-12] /* a(n-12) */
                      - 2304 * cache[i-14];/* a(n-14) */
            cache[i] = val;
        }
        cache_len = k;
    }

    return cache[n-1];                  /* convert 1‑based index to 0‑based */
}

/*
 * A218061  Number of nX5 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..1 nX5 array.
 * https://oeis.org/A218061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -56*a(n-2) +37*a(n-3) +117*a(n-4) -132*a(n-5)
 *            -128*a(n-6) +164*a(n-7) +60*a(n-8) -76*a(n-9) -4*a(n-10)
 *            +8*a(n-11) for n>13 Initial values:
 *            a(1)=10,a(2)=108,a(3)=1046,a(4)=10246,a(5)=99934,a(6)=975296,
 *            a(7)=9519284,a(8)=92918894,a(9)=907013406,a(10)=8853740672,
 *            a(11)=86425318934,a...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --- static cache for memoization --- */
static seq_t *cache   = NULL;
static idx_t cache_cap = 0;   /* allocated size */
static idx_t cache_size = 0;  /* filled entries count */

/* --- initial sequence values (1..13) --- */
static const seq_t init_vals[13] = {
    (seq_t)10,
    (seq_t)108,
    (seq_t)1046,
    (seq_t)10246,
    (seq_t)99934,
    (seq_t)975296,
    (seq_t)9519284,
    (seq_t)92918894,
    (seq_t)907013406,
    (seq_t)8853740672ULL,
    (seq_t)86425318934ULL,
    (seq_t)843636853718ULL,
    (seq_t)8235125319032ULL
};

/* ensure the cache can hold at least 'need' elements, computing
   missing values with the recurrence if necessary */
static void ensure_cache(idx_t need)
{
    /* grow if necessary */
    if (cache == NULL) {
        cache_cap = (need > 16) ? need * 2 : 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    } else if (need > cache_cap) {
        idx_t newcap = cache_cap * 2;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) abort();               /* OOM – terminate */
        cache = tmp;
        cache_cap = newcap;
    }

    /* fill initial values if not already present */
    if (cache_size < 13) {
        for (idx_t i = cache_size; i < 13; ++i)
            cache[i] = init_vals[i];
        cache_size = 13;
    }

    /* compute further entries using the 11‑term linear recurrence */
    const int coeff[11] = {15, -56, 37, 117, -132, -128, 164, 60, -76, -4, 8};
    for (idx_t i = cache_size; i < need; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < 11; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeff[k];
            if (c >= 0)
                sum += term * (seq_t)c;
            else
                sum -= term * (seq_t)(-c);
        }
        cache[i] = sum;
    }
    cache_size = need;
}

/* external function required by the spec */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – not used by OEIS */
    if (n <= 13) return init_vals[n - 1];

    ensure_cache(n);
    return cache[n - 1];
}

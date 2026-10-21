/*
 * A203734  Number of (n+1)X2 0..3 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -35*a(n-2) +35*a(n-3) +72*a(n-4) -178*a(n-5)
 *            +7*a(n-6) +259*a(n-7) -123*a(n-8) -182*a(n-9) +125*a(n-10)
 *            +68*a(n-11) -56*a(n-12) -13*a(n-13) +12*a(n-14) +a(n-15)
 *            -a(n-16) with initial values a(1)=90, a(2)=499, a(3)=2166,
 *            a(4)=8667, a(5)=31942, a(6)=111916, a(7)=376415, a(8)=122814...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    0ULL,                 /* index 0 unused */
    90ULL, 499ULL, 2166ULL, 8667ULL, 31942ULL, 111916ULL,
    376415ULL, 1228141ULL, 3911718ULL, 12225055ULL,
    37629475ULL, 114415963ULL, 344453875ULL, 1028640925ULL,
    3051602949ULL, 9004181108ULL
};

static seq_t *cache = NULL;          /* 1‑based cache, cache[0] unused */
static size_t cache_capacity = 0;    /* allocated size (incl. index 0) */
static idx_t cache_last = 0;         /* largest index computed */

/* enlarge cache so that indices up to n are valid */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 2;   /* at least n+1 indices + dummy 0 */
    if (cache_capacity < needed) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n <= 16)                /* base case, no caching required */
        return init_vals[n];

    ensure_cache(n);

    /* initialise early entries once */
    if (cache_last < 16) {
        for (idx_t i = cache_last + 1; i <= 16; ++i)
            cache[i] = init_vals[i];
        cache_last = 16;
    }

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)10   * cache[i-1];
        val -= (seq_t)35   * cache[i-2];
        val += (seq_t)35   * cache[i-3];
        val += (seq_t)72   * cache[i-4];
        val -= (seq_t)178  * cache[i-5];
        val += (seq_t)7    * cache[i-6];
        val += (seq_t)259  * cache[i-7];
        val -= (seq_t)123  * cache[i-8];
        val -= (seq_t)182  * cache[i-9];
        val += (seq_t)125  * cache[i-10];
        val += (seq_t)68   * cache[i-11];
        val -= (seq_t)56   * cache[i-12];
        val -= (seq_t)13   * cache[i-13];
        val += (seq_t)12   * cache[i-14];
        val += (seq_t)1    * cache[i-15];
        val -= (seq_t)1    * cache[i-16];
        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}

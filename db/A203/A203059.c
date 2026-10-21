/*
 * A203059  Number of nX1 0..6 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -21*a(n-2) +56*a(n-3) -70*a(n-4) +126*a(n-5)
 *            -84*a(n-6) +120*a(n-7) -45*a(n-8) +55*a(n-9) -11*a(n-10)
 *            +12*a(n-11) -a(n-12) +a(n-13) Initial values: a(1)=1, a(2)=7,
 *            a(3)=49, a(4)=252, a(5)=1127, a(6)=4977, a(7)=22736,
 *            a(8)=105813, a(9)=491533, a(10)=2269938, a(11)=10462235,
 *            a(12)=4...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* fixed initial terms, only needed for the first 13 indices */
static const seq_t init_terms[13] = {
    (seq_t)1,
    (seq_t)7,
    (seq_t)49,
    (seq_t)252,
    (seq_t)1127,
    (seq_t)4977,
    (seq_t)22736,
    (seq_t)105813,
    (seq_t)491533,
    (seq_t)2269938,
    (seq_t)10462235,
    (seq_t)48259083,
    (seq_t)222798408
};

/* cache for computed values */
static seq_t *cache = NULL;          /* array pointer */
static idx_t cache_len = 0;          /* number of elements stored (1‑based count) */
static idx_t cache_cap = 0;          /* allocated capacity */

/* ensure enough space in cache for 'needed' terms (1‑based) */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 13;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr && new_cap)
        abort();                     /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* initialise cache with the first 13 known terms, if not already done */
static void init_cache(idx_t n)
{
    if (cache)
        return;
    idx_t init_cap = n > 13 ? n : 13;
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!cache && init_cap)
        abort();                      /* out of memory */
    for (idx_t i = 0; i < 13; ++i)
        cache[i] = init_terms[i];
    cache_len = 13;
    cache_cap = init_cap;
}

/* extend cache up to index 'n' (1‑based) using the recurrence */
static void compute_to(idx_t n)
{
    if (n <= cache_len)
        return;
    for (idx_t i = cache_len; i < n; ++i) /* i is zero‑based index of a(i+1) */
    {
        seq_t val = 0;
        val += (seq_t)7   * cache[i-1];
        val -= (seq_t)21  * cache[i-2];
        val += (seq_t)56  * cache[i-3];
        val -= (seq_t)70  * cache[i-4];
        val += (seq_t)126 * cache[i-5];
        val -= (seq_t)84  * cache[i-6];
        val += (seq_t)120 * cache[i-7];
        val -= (seq_t)45  * cache[i-8];
        val += (seq_t)55  * cache[i-9];
        val -= (seq_t)11  * cache[i-10];
        val += (seq_t)12  * cache[i-11];
        val -= cache[i-12];
        val += cache[i-13];
        cache[i] = val;
    }
    cache_len = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n < 1 */
    init_cache(n);
    ensure_capacity(n);
    compute_to(n);
    return cache[n-1];
}

/*
 * A079982  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-1,0,1,2}.
 * https://oeis.org/A079982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+2*a(n-5)+3*a(n-6)-a(n-7)-3*a(n-8)+a(n-9)
 *            -a(n-10)-a(n-11)-3*a(n-12)+a(n-13)+3*a(n-14)-2*a(n-15)+a(n-18)-a(n-20)
 *            Initial values:
 *            a(0)=1,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=1,a(7)=0,a(8)=0,a(9)=0,
 *            a(10)=1,a(11)=4,a(12)=1,a(13)=1,a(14)=1,a(15)=1,a(16)=10,a(17)=10,
 *            a(18)=7,a(19)=7 T...
 * Range:     u8 n <= 28, u16 n <= 46, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[20] = {
    1,0,0,0,0,
    1,1,0,0,0,
    1,4,1,1,1,
    1,10,10,7,7
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure the cache can hold at least 'need' elements (0‑based). */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 20;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise cache with the first 20 terms on first call. */
    if (cache_size == 0) {
        ensure_cache(20);
        for (size_t i = 0; i < 20; ++i) cache[i] = init_vals[i];
        cache_size = 20;
    }

    if (idx < cache_size) return cache[idx];

    ensure_cache(idx + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t val = 0;

        if (i >= 2)  val += cache[i - 2];
        if (i >= 5)  val += (seq_t)2 * cache[i - 5];
        if (i >= 6)  val += (seq_t)3 * cache[i - 6];
        if (i >= 7)  val -= cache[i - 7];
        if (i >= 8)  val -= (seq_t)3 * cache[i - 8];
        if (i >= 9)  val += cache[i - 9];
        if (i >=10)  val -= cache[i -10];
        if (i >=11)  val -= cache[i -11];
        if (i >=12)  val -= (seq_t)3 * cache[i -12];
        if (i >=13)  val += cache[i -13];
        if (i >=14)  val += (seq_t)3 * cache[i -14];
        if (i >=15)  val -= (seq_t)2 * cache[i -15];
        if (i >=18)  val += cache[i -18];
        if (i >=20)  val -= cache[i -20];

        cache[i] = val;
    }

    cache_size = idx + 1;
    return cache[idx];
}

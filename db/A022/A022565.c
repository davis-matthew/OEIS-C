/*
 * A022565  Expansion of 1/((1-x)*(1-5*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A022565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*10^(n+3) - 30*8^(n+3) + 21*5^(n+3) -5)/1260
 *            Recurrence: a(0)=1, a(1)=24, a(2)=383, a(3)=5130
 *            a(n)=24*a(n-1)-193*a(n-2)+570*a(n-3)-400*a(n-4) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- generic small‑multiplication helper ----------
 * Use 128‑bit arithmetic for the 64‑bit case, otherwise fall back
 * to ordinary multiplication. This works in both C and C++.
 */
#ifdef __SIZEOF_INT128__
static inline seq_t mul_small(seq_t v, unsigned int m)
{
    /* If seq_t is 64‑bit or smaller, do the math in 128‑bit to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        __uint128_t tmp = (__uint128_t)v * m;
        return (seq_t)tmp;
    }
    /* For bigger (e.g. arbitrary‑precision) types use normal operator*. */
    return v * m;
}
#else
#  define mul_small(v,m) ((v) * (m))
#endif

/* ---------- cache management ----------
 * The cache is kept static and grows as needed.
 * In C++ we use new/delete; in C we use malloc/realloc.
 */
#ifdef __cplusplus
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cache_cap = 0; /* allocated size */
static size_t cache_len = 0; /* number of initialized elements */

/* Allocates room for at least 'need' elements.  Existing elements are copied. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* ---------- main function ----------
 * Computes a(n) using memoised recurrence.
 */
seq_t a(idx_t n)
{
    /* Ensure space for the requested index. */
    ensure_capacity((size_t)n + 1);

    /* Initialise the first four terms if not yet done. */
    if (cache_len < 4)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)383;
        cache[3] = (seq_t)5130;
        cache_len = 4;
    }

    /* Compute terms up to n. */
    for (; cache_len <= (size_t)n; ++cache_len)
    {
        seq_t t1 = mul_small(cache[cache_len-1], 24);
        seq_t t2 = mul_small(cache[cache_len-2], 193);
        seq_t t3 = mul_small(cache[cache_len-3], 570);
        seq_t t4 = mul_small(cache[cache_len-4], 400);
        cache[cache_len] = t1 - t2 + t3 - t4;
    }

    return cache[(size_t)n];
}

/*
 * A049898  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049898
   a(1) = 1 , a(2) = 1 , a(3) = 4
   For n ≥ 4:
       p = ⌊log₂(n–1)⌋          // 2ᵖ < n–1 ≤ 2^(p+1)
       m = n – 1 – 2ᵖ
       a(n) = (a(1)+…+a(n–1)) – a(m)
   The function is memoised and works for calls in any order.
   It is compatible with C11 and C++17.
*/
#include <stdlib.h>
#include <stdint.h>

/* Forward declarations */
static void ensure_cap(size_t need);
static unsigned floor_log2_u64(idx_t v);

/* Static storage for memoisation */
#ifdef __cplusplus
/* In C++ we use operator new[] so that constructors of seq_t run. */
static seq_t *cache_a   = nullptr;
static seq_t *cache_sum = nullptr;
#else
/* In C we use malloc/realloc – seq_t is a plain integer type. */
static seq_t *cache_a   = NULL;
static seq_t *cache_sum = NULL;
#endif
static size_t cache_cap = 0;  /* capacity of the arrays */
static size_t cache_len = 0;  /* number of terms currently computed (≥0) */

/* ------------------------------------------------------------------ */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    if (new_cap < need)
        new_cap = need;

#ifdef __cplusplus
    seq_t *new_a   = new seq_t[new_cap];
    seq_t *new_sum = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) {
        new_a[i]   = cache_a[i];
        new_sum[i] = cache_sum[i];
    }
    delete[] cache_a;
    delete[] cache_sum;
    cache_a   = new_a;
    cache_sum = new_sum;
#else
    seq_t *new_a   = (seq_t *)realloc(cache_a,   new_cap * sizeof(seq_t));
    seq_t *new_sum = (seq_t *)realloc(cache_sum, new_cap * sizeof(seq_t));
    cache_a   = new_a;
    cache_sum = new_sum;
#endif
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
static unsigned floor_log2_u64(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined, return 0 for safety */
        return (seq_t)0;

    /* lazy initialisation of the cache */
    if (cache_a == NULL)
    {
        cache_cap = 4;
        cache_len = 0;
#ifdef __cplusplus
        cache_a   = new seq_t[cache_cap];
        cache_sum = new seq_t[cache_cap];
#else
        cache_a   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sum = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        /* initial terms */
        cache_a[0] = (seq_t)1;          /* a(1) */
        cache_a[1] = (seq_t)1;          /* a(2) */
        cache_a[2] = (seq_t)4;          /* a(3) */

        cache_sum[0] = cache_a[0];
        cache_sum[1] = cache_sum[0] + cache_a[1];
        cache_sum[2] = cache_sum[1] + cache_a[2];
        cache_len = 3;                  /* we have a(1..3) */
    }

    /* return a cached value if already known */
    if (n <= cache_len)
        return cache_a[n - 1];

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i)
    {
        idx_t term_num = (idx_t)i + 1;          /* 1‑based index */
        /* determine m = term_num - 1 - 2^p, 2^p < term_num-1 <= 2^(p+1) */
        idx_t v  = term_num - 2;                /* >= 2 for term_num >= 4 */
        unsigned p  = floor_log2_u64(v);
        idx_t pow2 = (idx_t)1 << p;
        idx_t m   = term_num - 1 - pow2;       /* m >= 1 */
        size_t m_idx = (size_t)(m - 1);        /* zero‑based index */

        ensure_cap(i + 1);                      /* make room for the i‑th value */

        /* a(n) = sum_{k=1}^{n-1} a(k) - a(m) */
        seq_t val = cache_sum[i - 1] - cache_a[m_idx];
        cache_a[i]   = val;
        cache_sum[i] = cache_sum[i - 1] + val;
    }

    cache_len = (size_t)n;
    return cache_a[n - 1];
}

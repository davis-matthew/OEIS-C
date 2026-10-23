/*
 * A049920  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * Sum_{i=1}^{n-1} a(i) - a(A006257(n-2)) * with a(1)=1,
 *            a(2)=3, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;         /* computed a(i)-values */
static idx_t   cache_len = 0;         /* current number of computed values */
static idx_t   cache_cap = 0;         /* capacity of a_cache */
static seq_t   prefix_sum = 0;        /* sum of a(1) .. a(cache_len) */

/* Return the smallest power of 2 that is >= n */
static idx_t next_pow2(idx_t n)
{
    if (n <= 1) return 1;
    n -= 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#if ULONG_MAX > 0xFFFFFFFFUL
    n |= n >> 32;
#endif
    return n + 1;
}

/* Grow the cache to accommodate at least 'needed' elements */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but defensive */
    while (cache_len < n) {
        idx_t i = cache_len + 1;           /* next index to compute (1‑based) */
        seq_t val;

        if (i == 1)          val = (seq_t)1;
        else if (i == 2)     val = (seq_t)3;
        else if (i == 3)     val = (seq_t)2;
        else {
            /* Sum_{j=1}^{i-1} a(j)  is prefix_sum */
            seq_t prefix = prefix_sum;
            /* compute m = 2*i - 3 - 2^(p+1), where 2^(p+1) is the smallest power of two >= i-1 */
            idx_t s_idx = next_pow2(i - 1);
            seq_t s = (seq_t)s_idx;
            seq_t m_val = (seq_t)2 * (seq_t)i - (seq_t)3 - s;
            idx_t m = (idx_t)m_val;          /* m is guaranteed < i for n>=4 */
            seq_t a_m = a_cache[m - 1];
            val = prefix - a_m;
        }

        ensure_cache_capacity(cache_len + 1);
        a_cache[cache_len] = val;
        cache_len++;
        prefix_sum += val;
    }
    return a_cache[n - 1];
}

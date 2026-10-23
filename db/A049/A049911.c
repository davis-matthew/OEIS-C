/*
 * A049911  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2 and a(3) = 3.
 * https://oeis.org/A049911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049911
   a(1)=1, a(2)=2, a(3)=3
   a(n)=a(1)+…+a(n-1)-a(m)   (n≥4)
   where m=2*n-2-2^(p+1) and p is the unique integer such that
   2^p < n-1 ≤ 2^(p+1).  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"   /* provides floor_log2_uint64 */

static seq_t *cache = NULL;           /* 0‑based array of a(i) */
static size_t capacity = 0;           /* allocated capacity of cache   */
static idx_t cached_len = 0;          /* number of stored terms (up to a(cached_len)) */
static seq_t cum_sum = 0;             /* sum of a(1)…a(cached_len)    */

/* enlarge cache if necessary (realloc works for both C and C++) */
static void grow_cache(size_t needed)
{
    if (capacity < needed) {
        size_t newcap = capacity ? capacity * 2 : 32;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        capacity = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* not used, but safe */

    /* compute terms up to n if required */
    if (cached_len < n) {
        grow_cache(n);
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)2;
            } else if (i == 3) {
                val = (seq_t)3;
            } else {
                /* recurrence for i ≥ 4 */
                idx_t t = i - 1;                       /* n‑1 */
                idx_t p_log = floor_log2_uint64(t);
                /* 2^p < t ≤ 2^(p+1)  →  p = (t is a power of two) ? p_log−1 : p_log */
                idx_t p;
                if ((t & (t - 1)) == 0) {              /* t is power of two */
                    p = p_log - 1;
                } else {
                    p = p_log;
                }
                idx_t power = ((idx_t)1) << (p + 1);    /* 2^(p+1) */
                idx_t m = 2 * i - 2 - power;           /* 1‑based index to subtract */
                val = cum_sum - cache[m - 1];
            }
            cache[cached_len] = val;
            cum_sum += val;
            ++cached_len;
        }
    }
    return cache[n - 1];
}

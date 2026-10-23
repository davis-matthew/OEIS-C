/*
 * A049953  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A049953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = a(3) = 2;
   for n ≥ 4,
     a(n) = a(1) + a(2) + … + a(n‑1) + a(m)
   where m = 2^(p+1) + 2 − n,
   and p is the unique integer such that 2^p < n‑1 ≤ 2^(p+1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

/* Cache for computed a(n) values and cumulative sums S(n)=a(1)+…+a(n). */
static seq_t *a_cache = NULL;      /* 1‑based index  */
static seq_t *S_cache = NULL;      /* cumulative sums  */
static idx_t cache_len = 0;        /* number of terms computed so far   */
static idx_t cache_cap = 0;        /* current allocated capacity        */

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, harmless return */

    /* Ensure the cache can hold indices up to n. */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : n + 1;
        if (new_cap < n + 1) new_cap = n + 1;
        a_cache  = (seq_t *)realloc(a_cache, new_cap * sizeof(*a_cache));
        S_cache  = (seq_t *)realloc(S_cache, new_cap * sizeof(*S_cache));
        cache_cap = new_cap;
    }

    /* Compute missing terms sequentially. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        if (k == 1) {                      /* base case */
            a_cache[1] = (seq_t)1;
            S_cache[1] = a_cache[1];
        } else if (k == 2 || k == 3) {      /* a(2)=a(3)=2 */
            a_cache[k] = (seq_t)2;
            S_cache[k] = S_cache[k - 1] + a_cache[k];
        } else {
            /* Determine p such that 2^p < k-1 ≤ 2^(p+1). */
            idx_t p = floor_log2_uint64(k - 2);
            /* m = 2^(p+1) + 2 - k  (m < k) */
            seq_t two_pow = pow2(p + 1);          /* 2^(p+1) as seq_t */
            seq_t m_val   = two_pow + (seq_t)2 - (seq_t)k;
            idx_t m = (idx_t)m_val;
            /* a(k) = S(k-1) + a(m) */
            seq_t a_k = S_cache[k - 1] + a_cache[m];
            a_cache[k] = a_k;
            S_cache[k] = S_cache[k - 1] + a_k;
        }
    }
    cache_len = n;  /* update length */
    return a_cache[n];
}

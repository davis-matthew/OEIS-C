/*
 * A049965  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A049965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A049965 – a(n) = a(1)+…+a(n‑1)+a(m)  (n≥4)
    Base: a(1)=1, a(2)=3, a(3)=1
    m = 2^(p+1)+2–n  ,  p = ⌊log₂(n–2)⌋
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

/* Memoisation caches */
/* a_cache[1…]  holds the sequence terms   */
/* pref_cache[0…] holds prefix sums (pref_cache[0]=0) */
static seq_t *a_cache   = NULL;
static seq_t *pref_cache = NULL;
static idx_t cache_size = 0;   /* largest index that has been computed */

/* Ensure that indices up to n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already available */

    /* Grow the arrays to accommodate indices 0 … n */
    size_t new_size = (size_t)(n + 1);
    a_cache      = (seq_t *)realloc(a_cache,      new_size * sizeof(seq_t));
    pref_cache   = (seq_t *)realloc(pref_cache,   new_size * sizeof(seq_t));
    if (pref_cache) pref_cache[0] = (seq_t)0;   /* prefix[0] = 0 */

    /* Compute remaining terms */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = (seq_t)1;
        } else if (k == 2) {
            val = (seq_t)3;
        } else if (k == 3) {
            val = (seq_t)1;
        } else {
            /* p = ⌊log₂(k−2)⌋  */
            unsigned p = floor_log2_uint64(k - 2ULL);
            idx_t two_pow = ((idx_t)1U << (p + 1));     /* 2^(p+1) */
            idx_t m = two_pow + 2ULL - k;              /* m < k   */
            val = pref_cache[k - 1] + a_cache[m];
        }
        a_cache[k]      = val;
        pref_cache[k]   = pref_cache[k - 1] + val;
    }
    cache_size = n;
}

/* Return the n‑th term of the sequence (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n<1, but silence compiler */
    ensure_cache(n);
    return a_cache[n];
}

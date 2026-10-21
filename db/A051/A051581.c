/*
 * A051581  a(n) = (2*n+7)!!/7!!, related to A001147 (odd double factorials).
 * https://oeis.org/A051581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+7)!! / 7!!. Recurrence: a(0) = 1, a(n+1) = (2*n+9) *
 *            a(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoised multiplication to compute any a(n) in
   O(n) time relative to the largest n requested.  All operations
   are performed with the seq_t type, so the implementation works
   for 8‑, 16‑, 32‑, 64‑bit builds as well as for arbitrary‑precision
   builds where seq_t is a big‑integer class.  */
#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;          /* memoisation array */
static idx_t cache_len = 0;          /* number of elements stored */
static idx_t cache_cap = 0;          /* capacity of the array */

/* Ensure the cache contains values up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    while (cache_len <= n) {
        seq_t value;
        if (cache_len == 0) {
            /* a(0) = 1 */
            value = (seq_t)1;
        } else {
            /* factor = 2*cache_len + 7 , the multiplier for a(cache_len) */
            idx_t factor = (idx_t)(2 * cache_len + 7);
            value = cache[cache_len - 1] * ((seq_t)factor);
        }

        if (cache_len == cache_cap) {
            idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* In a constrained environment malloc failure is unlikely
               for the sizes we need; if it happens, the run will abort
               naturally.  */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = value;
        ++cache_len;
    }
}

/* Public function required by the framework.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

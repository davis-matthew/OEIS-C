/*
 * A039124  Numbers whose base-10 representation has the same number of 1's
 *          and 6's.
 * https://oeis.org/A039124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_1_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: numbers whose decimal representation
   has the same number of 1's and 6's. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_1_6.h"

/*--- static state ----------------------------------------------------*/
static seq_t *cache      = NULL;   /* cached terms */
static size_t cached_len = 0;      /* number of cached terms */
static idx_t next_scan   = 0;      /* next integer to test */

/*--- helper: ensure capacity of cache -------------------------------*/
static inline void ensure_cache_cap(size_t needed)
{
    size_t cap = cached_len;
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cached_len; ++i)
                tmp[i] = cache[i];
            delete[] cache;
        }
#else
        tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return;          /* allocation failed – caller will retry */
        cache = tmp;
    }
}

/*--- OEIS function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset is irrelevant */

    /* if already computed, return from cache */
    if (n <= cached_len)
        return cache[n - 1];

    /* generate further terms until we reach the n‑th one */
    for (idx_t cur = next_scan; ; ++cur) {
        seq_t candidate = (seq_t)cur; /* cast is safe for small seq_t builds */
        if (has_equal_1_6(candidate)) {
            ensure_cache_cap(cached_len + 1);
            cache[cached_len++] = candidate;
            if (cached_len == n) {
                next_scan = cur + 1;
                return candidate;
            }
        }
    }
}

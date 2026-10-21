/*
 * A205014  s(k)-s(j), where (s(k),s(j)) is the least pair of central binomial
 *          coefficients for which n divides their difference.
 * https://oeis.org/A205014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least positive difference s(k)-s(j) of central binomial
   coefficients C(2k,k)-C(2j,j) such that n divides the difference. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* static cache for computed values */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;

/* static array of central binomial coefficients s[m] = C(2m,m) */
static seq_t *s_arr = NULL;
static size_t s_cap = 2048;   /* initial capacity (will grow if needed) */
static size_t s_len = 0;     /* number of values stored (next index to compute) */

/* ensure that s_arr contains values up to index k (inclusive) */
static void ensure_s(idx_t k)
{
    if (!s_arr) {                     /* first time */
        s_arr = alloc_seq_array(s_cap);
        for (size_t i = 0; i < s_cap; ++i) s_arr[i] = 0;
        s_arr[0] = 1;                 /* C(0,0) */
        s_len = 1;
    }
    while (s_len <= (size_t)k) {
        /* grow array if necessary */
        if (s_len >= s_cap) {
            size_t new_cap = s_cap * 2;
            seq_t *new_arr = alloc_seq_array(new_cap);
            for (size_t i = 0; i < new_cap; ++i) new_arr[i] = 0;
            for (size_t i = 0; i < s_cap; ++i) new_arr[i] = s_arr[i];
            free_seq_array(s_arr);
            s_arr = new_arr;
            s_cap = new_cap;
        }
        idx_t idx = (idx_t)(s_len);
        seq_t mult = (seq_t)4 * (seq_t)idx - (seq_t)2;
        s_arr[(size_t)idx] = s_arr[(size_t)(idx-1)] * mult / (seq_t)idx;
        ++s_len;
    }
}

/* ensure that cache has room for index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 1024 : cache_cap * 2;
    while ((size_t)(n + 1) > new_cap) new_cap *= 2;
    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < new_cap; ++i) new_arr[i] = 0;
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_arr[i] = cache[i];
        free_seq_array(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe */
    if (n == 1) return (seq_t)1;       /* a(1) = 1 */

    ensure_cache(n);
    if (cache[(size_t)n] != 0) return cache[(size_t)n];

    seq_t best_val = 0;
    bool  found    = false;

    for (idx_t k = 1; ; ++k) {
        ensure_s(k);

        for (idx_t j = 0; j < k; ++j) {
            seq_t diff = s_arr[(size_t)k] - s_arr[(size_t)j];
            if (diff % (seq_t)n == 0) {
                if (!found || diff < best_val) {
                    best_val = diff;
                    found    = true;
                }
            }
        }

        if (found) {
            seq_t diff_k = s_arr[(size_t)k] - s_arr[(size_t)(k - 1)];
            if (diff_k > best_val) break;   /* future differences are larger */
        }
    }

    cache[(size_t)n] = best_val;
    return best_val;
}

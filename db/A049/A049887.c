/*
 * A049887  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A049887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+...+a(n-1) - a(m) for n ≥ 4 where a(1)=a(2)=a(3)=1 m
 *            = 2*n - 2 - 2^(p+1) p is the unique integer such that 2^p < n-1
 *            ≤ 2^(p+1) (equivalently, p = floor_log2(n-2) )
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return floor(log2(v)) for v > 0.  Uses builtin if available,
   otherwise falls back to a simple loop. */
static inline idx_t floor_log2_u64(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(v);
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* Compute the n‑th term of A049887.  Memoises computed values
   in static arrays; handles calls in any order. */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *sum_cache = NULL;
    static idx_t cache_size = 0;

    /* expand cache so that indices up to n are available */
    if (n > cache_size) {
        idx_t old_size = cache_size;
        cache_size = n;

        a_cache   = (seq_t*)realloc(a_cache, (cache_size + 1) * sizeof(seq_t));
        sum_cache = (seq_t*)realloc(sum_cache, (cache_size + 1) * sizeof(seq_t));

        if (old_size == 0) {
            sum_cache[0] = (seq_t)0;
        }

        for (idx_t i = old_size + 1; i <= cache_size; ++i) {
            if (i <= 3) {
                a_cache[i] = (seq_t)1;
            } else {
                /* compute p = floor_log2(n-2) */
                idx_t p   = floor_log2_u64(i - 2);
                idx_t pow = ((idx_t)1) << (p + 1);
                idx_t m   = 2 * i - 2 - pow;
                a_cache[i] = sum_cache[i - 1] - a_cache[m];
            }
            sum_cache[i] = sum_cache[i - 1] + a_cache[i];
        }
    }

    return a_cache[n];
}

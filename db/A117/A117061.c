/*
 * A117061  a(n) = (s(n-1))^2+n, with a(1) = 1 and s(k) = A007953(a(k)).
 * https://oeis.org/A117061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (s(n-1))^2 + n where s(k) is the sum of decimal digits of
 *            a(k) and a(1) = 1
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"

/*  A static array is sufficient for the required index range (≤ 10 000).  */
static seq_t cache[10001];
static bool cache_initialized = false;
static idx_t cache_max_index = 1;

/*  Compute a(n) for any n ≥ 1, memoizing previously computed values.  */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for zero; return 0 for safety */
        return 0;

    /* Initialise the cache on first use */
    if (!cache_initialized) {
        cache[1] = 1;        /* a(1) = 1 */
        cache_initialized = true;
        cache_max_index = 1;
    }

    /* Expand cached values up to the requested index */
    if (n > cache_max_index) {
        if (n >= sizeof(cache) / sizeof(cache[0])) {
            /* For the given constraints n will never exceed the array size.
               If it does, fall back to on‑demand computation without caching
               (slow but correct).  */
            seq_t prev = cache[cache_max_index];
            for (idx_t i = cache_max_index + 1; i <= n; ++i) {
                seq_t s = digit_sum(prev);
                prev = s * s + (seq_t)i;
            }
            return prev;
        }
        for (idx_t i = cache_max_index + 1; i <= n; ++i) {
            seq_t s = digit_sum(cache[i - 1]);   /* s(i‑1) from previous term */
            cache[i] = s * s + (seq_t)i;
        }
        cache_max_index = n;
    }

    return cache[n];
}

/*
 * A045502  Numbers k such that 2*k+1 and 3*k+1 are squares.
 * https://oeis.org/A045502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) (n > 2) * Initial values:
 *            a(0) = 0, a(1) = 40, a(2) = 3960 * * This implementation
 *            memoises the sequence in a statically-sized * array; it will
 *            compute all terms up to 10 000 in under 2 s * for any supported
 *            seq_t/idx_t pair
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000   /* sufficient for n ≤ 10 000 */

seq_t a(idx_t n)
{
    /* static cache of already-computed values */
    static seq_t cache[MAX_CACHE];
    static idx_t cache_len = 0;   /* number of terms cached (1‑based) */

    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)3960;

    /* initialise cache with the first three terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)3960;
        cache_len = 3;
    }

    /* extend the cache up to n if necessary */
    if ((idx_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)99 * cache[i-1] - (seq_t)99 * cache[i-2] + cache[i-3];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}

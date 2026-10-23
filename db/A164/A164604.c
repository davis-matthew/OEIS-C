/*
 * A164604  a(n) = ((1+4*sqrt(2))*(3+2*sqrt(2))^n +
 *          (1-4*sqrt(2))*(3-2*sqrt(2))^n)/2.
 * https://oeis.org/A164604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=1, a(1)=19
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 10001

extern seq_t a(idx_t n)
{
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0; /* number of terms stored; 0 means uninitialized */

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache_len = 2;
    }

    /* Use the cache when the requested index is within the pre‑allocated limit. */
    if ((size_t)n + 1 <= MAX_CACHE) {
        if (cache_len <= (size_t)n) {
            for (idx_t i = cache_len; i <= n; ++i) {
                cache[i] = (seq_t)6 * cache[i - 1] - cache[i - 2];
            }
            cache_len = (size_t)n + 1;
        }
        return cache[n];
    }

    /* Compute the value on the fly when n exceeds the cache size. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)19;

    seq_t a_nm2 = (seq_t)1;
    seq_t a_nm1 = (seq_t)19;
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n   = (seq_t)6 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

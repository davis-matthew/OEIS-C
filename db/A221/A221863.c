/*
 * A221863  Expansion of (1-3*x+x^2)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A221863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5);
 *            a(0)=1, a(1)=6, a(2)=27, a(3)=110, a(4)=429
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t cache[10001];
    static size_t cache_len = 0;

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0)         cache[i] = (seq_t)1;
        else if (i == 1)    cache[i] = (seq_t)6;
        else if (i == 2)    cache[i] = (seq_t)27;
        else if (i == 3)    cache[i] = (seq_t)110;
        else if (i == 4)    cache[i] = (seq_t)429;
        else
            cache[i] = (seq_t)9   * cache[i-1]
                     - (seq_t)28  * cache[i-2]
                     + (seq_t)35  * cache[i-3]
                     - (seq_t)15  * cache[i-4]
                     +      cache[i-5];
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

/*
 * A052972  Expansion of (1-x^3)/(1-x-x^2-x^3+x^5).
 * https://oeis.org/A052972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-5) for n≥5
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6 */
#include <stdlib.h>

static seq_t cache[10001];
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)6;
        cache_size = 5;
    }
    if (n < cache_size)
        return cache[n];

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-5];
    }
    cache_size = n + 1;
    return cache[n];
}

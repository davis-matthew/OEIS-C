/*
 * A076307  a(n) = n+min(2*floor(a(n-1)/2), 3*floor(a(n-1)/3)) for n > 1,
 *          a(1)=1.
 * https://oeis.org/A076307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + min( 2*floor(a(n-1)/2), 3*floor(a(n-1)/3) ), a(1) = 1
 *            (offset 1)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is known to be required only up to about 10 000.
       A static array allows memoisation without dynamic allocation,
       avoiding construction issues for arbitrary-precision types.   */
    static seq_t cache[10001];
    static idx_t last = 0;          /* last index that has been computed */

    /* Initialise on first entry */
    if (last == 0) {
        cache[1] = (seq_t)1;
        last = 1;
    }

    if (n <= last)
        return cache[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t two  = 2 * (prev / 2);
        seq_t three = 3 * (prev / 3);
        seq_t m = (two < three) ? two : three;
        cache[i] = (seq_t)i + m;
    }
    last = n;
    return cache[n];
}

/*
 * A021052  Decimal expansion of 1/48.
 * https://oeis.org/A021052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/48) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth decimal digit of the number 1/48. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache the computed digits as small bytes. */
    static uint8_t digits[200000];
    static size_t cache_size = 0;     /* number of digits currently cached */
    static idx_t rem = 1;             /* remainder after last computed digit (mod 48) */

    /* If the requested index is within the pre‑allocated cache range, use it. */
    if (n < cache_size) {
        return (seq_t)digits[n];
    }

    /* If n >= cache_size and n is still within the allocated buffer, compute
       missing digits and store them.  The loop is O(n) overall and works for
       any order of calls. */
    if (n < 200000) {
        for (size_t i = cache_size; i <= n; ++i) {
            rem *= 10;
            digits[i] = (uint8_t)(rem / 48);  /* digit 0‑9 */
            rem %= 48;
        }
        cache_size = n + 1;
        return (seq_t)digits[n];
    }

    /* For indices beyond the buffer (unlikely in tests), compute directly. */
    idx_t r = 1, d = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        d = r / 48;
        r %= 48;
    }
    return (seq_t)d;
}

/*
 * A037913  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*10^i is the base-10 representation of n.
 * https://oeis.org/A037913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of positions i (i≥1) in the decimal representation
 *            of n such that |digit_i - digit_{i-1}| = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n == 0) {
        return count;   /* Undefined for n<1, but return 0 */
    }

    uint64_t x = (uint64_t)n;
    int last  = (int)(x % 10);
    x /= 10;

    while (x > 0) {
        int cur   = (int)(x % 10);
        x /= 10;

        int diff = (cur > last) ? (cur - last) : (last - cur);
        if (diff == 1) {
            count++;
        }

        last = cur;
    }

    return count;
}

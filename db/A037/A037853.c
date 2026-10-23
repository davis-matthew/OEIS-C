/*
 * A037853  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*3^i:
 *          i=0,1,...,m} is base 3 representation of n.
 * https://oeis.org/A037853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where
 *            Sum{d(i)*3^i: i=0,1,...,m} is the base‑3 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t t = n;
    unsigned prev = t % 3;   /* digit at position 0 */
    t /= 3;                  /* move to next higher digit */
    seq_t sum = 0;

    while (t > 0) {
        unsigned cur = t % 3;
        if (cur > prev) {
            sum += (seq_t)(cur - prev);
        }
        prev = cur;
        t /= 3;
    }
    return sum;
}

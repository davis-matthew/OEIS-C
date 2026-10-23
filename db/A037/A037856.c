/*
 * A037856  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*6^i:
 *          i=0,1,...,m} is base 6 representation of n.
 * https://oeis.org/A037856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{d(i)-d(i-1) : d(i)>d(i-1), i=1,...,m} where
 *            Sum{d(i)*6^i : i=0,1,...,m} is the base‑6 representation of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 6) return 0;                 /* single‑digit base‑6 numbers have no increases */

    idx_t tmp = n;
    uint8_t prev = (uint8_t)(tmp % 6);
    tmp /= 6;

    while (tmp > 0) {
        uint8_t cur = (uint8_t)(tmp % 6);
        if (cur > prev)
            sum += (seq_t)(cur - prev);
        prev = cur;
        tmp /= 6;
    }
    return sum;
}

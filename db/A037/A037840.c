/*
 * A037840  a(n)=Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*8^i:
 *          i=0,1,...,m} is the base 8 representation of n.
 * https://oeis.org/A037840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{|d(i)-d(i-1)|: i=1,...,m} where Sum{d(i)*8^i} = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* n ≥ 1 for the official range, but guard anyway */
        return 0;

    seq_t sum = 0;           /* running total of absolute differences */
    idx_t  temp = n;         /* work on a copy of the index */

    uint8_t prev = temp % 8; /* least‑significant base‑8 digit */
    temp /= 8;               /* shift to next digit */

    while (temp > 0) {
        uint8_t curr = temp % 8;
        sum += (seq_t)((prev > curr) ? (prev - curr) : (curr - prev));
        prev = curr;
        temp /= 8;
    }

    return sum;
}

/*
 * A063720  Number of segments lit in a 7-segment display (as on a calculator)
 *          to represent the number n, variant 0: '6', '7' and '9' use 5, 3
 *          and 5 segments, respectively.
 * https://oeis.org/A063720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(0)=6; for n>0, a(n)=a(n/10)+s(n%10) with
 *            s=[6,2,5,5,4,5,5,3,7,5]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static const unsigned char seg_counts[10] = {6,2,5,5,4,5,5,3,7,5};
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    seq_t sum = 0;
    while (n > 0) {
        unsigned int d = (unsigned int)(n % 10);
        sum += (seq_t)seg_counts[d];
        n /= 10;
    }
    return sum;
}

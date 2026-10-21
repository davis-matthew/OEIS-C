/*
 * A211199  Sum of the 16th powers of the decimal digits of n.
 * https://oeis.org/A211199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{digits d of n} d^16
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t POW16[10] = {
    0ULL,
    1ULL,
    65536ULL,
    43046721ULL,
    4294967296ULL,
    152587890625ULL,
    2821109907456ULL,
    33232930569601ULL,
    281474976710656ULL,
    1853020188851841ULL
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    if (t == 0) return POW16[0]; /* 0^16 = 0 */
    while (t > 0) {
        unsigned d = t % 10;
        sum += POW16[d];
        t /= 10;
    }
    return sum;
}

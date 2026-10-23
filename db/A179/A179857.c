/*
 * A179857  Smallest number greater than n having in binary representation
 *          exactly twice the number of ones as n has in binary
 *          representation.
 * https://oeis.org/A179857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m > n with popcount(m) = 2 * popcount(n)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        c++;
        x &= x - 1;   /* clear the lowest set bit */
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* guard against offset < 1 */

    seq_t sn = (seq_t)n;
    unsigned target = popcount_seq(sn) * 2;
    seq_t m = sn + 1;

    while (popcount_seq(m) != target) {
        ++m;
    }
    return m;
}

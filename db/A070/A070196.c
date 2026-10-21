/*
 * A070196  a(n) = n plus the sorted version of the base-10 digits of n.
 * https://oeis.org/A070196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + A004185(n) (sum of n and its sorted decimal digits)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Compute A070196: a(n) = n + sorted version of the decimal digits of n.
 * Sorting is performed in ascending order.
 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to result type */
    unsigned char freq[10] = {0};
    uint64_t x = n;

    /* Count the digits of n */
    if (x == 0) {
        freq[0] = 1;
    } else {
        while (x > 0) {
            freq[x % 10]++;
            x /= 10;
        }
    }

    /* Build the sorted number from the frequency table */
    seq_t sorted = 0;
    for (int d = 0; d < 10; ++d) {
        unsigned char c = freq[d];
        while (c--) {
            sorted = sorted * 10 + d;
        }
    }

    return nn + sorted;
}

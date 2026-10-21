/*
 * A222248  In the number n, replace all (decimal) digits '5' with '9' and
 *          vice versa.
 * https://oeis.org/A222248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace digits '5' with '9' and vice versa in decimal
 *            representation of n
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned char digs[20];   /* enough for uint64_t max (20 digits) */
    size_t pos = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digs[pos++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    seq_t result = 0;
    for (size_t i = pos; i > 0; --i) {
        unsigned char d = digs[i - 1];
        unsigned char mapped = (d == 5 ? 9 : (d == 9 ? 5 : d));
        result = result * 10 + (seq_t)mapped;
    }
    return result;
}

/*
 * A035044  Exchange 2 and 3.
 * https://oeis.org/A035044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal integer obtained by exchanging digits 2 ↔ 3 in
 *            the base‑10 representation of n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t tmp = n;
    unsigned char rev[20];
    int pos = 0;

    while (tmp > 0) {
        unsigned d = tmp % 10;
        if (d == 2)
            d = 3;
        else if (d == 3)
            d = 2;
        rev[pos++] = (unsigned char)d;
        tmp /= 10;
    }

    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i) {
        res = res * 10 + rev[i];
    }
    return res;
}

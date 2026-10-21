/*
 * A095496  a(n) = 52 written in base n.
 * https://oeis.org/A095496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 52 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* unary representation for base 1 */
    if (n == 1) {
        seq_t r = 0;
        for (uint64_t i = 0; i < 52; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* digits of 52 in base n (least significant first) */
    uint64_t tmp = 52;
    uint64_t digs[64];          /* 64 > maximum needed (6 digits for n ≤ 2) */
    size_t sz = 0;
    while (tmp > 0) {
        digs[sz++] = tmp % n;
        tmp /= n;
    }

    /* build the concatenated decimal number from most to least significant digit */
    seq_t r = 0;
    for (size_t i = sz; i-- > 0; ) {
        uint64_t d = digs[i];
        if (d < 10)
            r = r * 10 + (seq_t)d;
        else
            r = r * 100 + (seq_t)d;   /* d uses two decimal digits (10‑52) */
    }
    return r;
}

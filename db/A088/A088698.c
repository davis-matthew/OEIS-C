/*
 * A088698  Replace 1 with 11 in binary representation of n.
 * https://oeis.org/A088698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 0, a(2n) = 2*a(n), a(2n+1) = 4*a(n) + 3
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the integer obtained by replacing every 1 in the binary expansion of n with 11. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Find the index of the most significant set bit.  For n==0 this is 0. */
    idx_t tmp = n;
    int msb = 0;
    while ((tmp >>= 1) != 0)
        msb++;

    /* Scan bits from most significant to least significant,
       applying the recurrence: if bit==0, *2; if bit==1, *4+3. */
    for (int i = msb; i >= 0; --i) {
        if ((n >> i) & 1ULL)
            res = res * 4 + 3;   /* append bits "11" */
        else
            res = res * 2;       /* append bit "0" */
    }

    return res;
}

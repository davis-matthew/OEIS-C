/*
 * A208279  Central terms of Pascal's triangle mod 10 (A008975).
 * https://oeis.org/A208279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial(2*n,n) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned int binom5[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,2,1,0,0},
    {1,3,3,1,0},
    {1,4,1,4,1}
};

seq_t a(idx_t n)
{
    /* Compute C(2n,n) modulo 5 using Lucas' theorem in base 5. */
    unsigned int r5 = 1;
    idx_t tmp = n;
    unsigned int carry = 0;
    while (tmp > 0 || carry > 0) {
        unsigned int ni = (unsigned int)(tmp % 5);
        tmp     /= 5;
        unsigned int bi  = ni * 2 + carry;     /* value for current base‑5 digit of 2n */
        unsigned int d2  = bi % 5;             /* digit of 2n in this position */
        carry = bi / 5;                       /* carry to the next higher digit   */
        r5 = (r5 * binom5[d2][ni]) % 5;       /* multiply by C(d2,ni) mod 5 */
    }

    /* Combine modulo 2 and modulo 5 using the Chinese Remainder Theorem. */
    int need_parity = (n == 0) ? 1 : 0;       /* odd for n==0, even otherwise */
    seq_t result = (seq_t)((r5 & 1) == need_parity ? r5 : r5 + 5);
    return result;
}

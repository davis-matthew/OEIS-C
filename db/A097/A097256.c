/*
 * A097256  Numbers whose set of base 10 digits is {0,9}.
 * https://oeis.org/A097256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 * A007088(n). For n>0, write its binary representation
 *            b_k…b_0 (k = ⌊log₂ n⌋). Then a(n) = ∑_{i=0}^{k} b_i * 9 *
 *            10^{k-i}. The implementation builds the decimal number composed
 *            of digits 0 or 9 by scanning the bits of n from most significant
 *            to least
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find index of most significant bit */
    idx_t msb = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        msb++;
    }

    seq_t res = 0;
    for (idx_t i = msb;; --i) {
        res = res * 10;
        if (n & ((idx_t)1 << i))
            res += 9;
        if (i == 0)
            break;
    }
    return res;
}

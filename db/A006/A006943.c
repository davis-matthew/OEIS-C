/*
 * A006943  Rows of Sierpiński's triangle (Pascal's triangle mod 2).
 * https://oeis.org/A006943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{i=0..n} (binomial(n,i) mod 2) * 10^i = Product_{k :
 *            bit k of n is 1} (10^(2^k) + 1), i.e. the row read in base 2 is
 *            written out as a decimal number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t f = 10;            /* f = 10^(2^k) */
    idx_t m = n;
    while (m) {
        if (m & 1)
            result *= (f + 1);
        m >>= 1;
        if (m)
            f *= f;
    }
    return result;
}

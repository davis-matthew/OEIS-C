/*
 * A131650  Number of symbols in Babylonian numeral representation of n.
 * https://oeis.org/A131650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ ( floor(d_i/10) + (d_i mod 10) ) for the base‑60 digits
 *            d_i of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        idx_t d = n % 60;
        sum += (seq_t)(d / 10 + d % 10);
        n /= 60;
    }
    return sum;
}

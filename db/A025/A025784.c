/*
 * A025784  Expansion of 1/((1-x)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A025784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 16*n + 8*(2+(n mod 7))*(7-(n mod 7)) - 7*(n mod
 *            8)*(8-(n mod 8)))/112, for n ≥ 0
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned long long nn = (unsigned long long)n;
    unsigned long long m7 = (unsigned long long)(n % (idx_t)7);
    unsigned long long m8 = (unsigned long long)(n % (idx_t)8);

    unsigned long long term1 = nn * nn;
    unsigned long long term2 = 16 * nn;
    unsigned long long term3 = 8 * ((unsigned long long)2 + m7) * ((unsigned long long)7 - m7);
    unsigned long long term4 = 7 * m8 * ((unsigned long long)8 - m8);

    unsigned long long num = term1 + term2 + term3 - term4;
    return (seq_t)(num / 112);
}

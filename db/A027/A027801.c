/*
 * A027801  a(n) = 5*(n+1)*binomial(n+4,5)/2.
 * https://oeis.org/A027801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)*binomial(n+4,5)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t bin = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        bin = bin * (seq_t)(n + 4 - (i - 1));
        bin /= (seq_t)i;
    }
    seq_t res = bin;
    res *= (seq_t)(n + 1);
    res *= 5;
    res /= 2;
    return res;
}

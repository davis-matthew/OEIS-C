/*
 * A025785  Expansion of 1/((1-x)(1-x^7)(1-x^9)).
 * https://oeis.org/A025785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=0}^{n/7} ((n-7*j)/9 + 1)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t j = 0; j <= n / 7; ++j) {
        seq_t r = (seq_t)n - (seq_t)7 * j;
        seq_t c = r / 9;
        res += c + 1;
    }
    return res;
}

/*
 * A121200  a(n) = 2*n + 7^n + 5^n.
 * https://oeis.org/A121200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 7^n + 5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t pow5 = 1;
    seq_t pow7 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow7 *= (seq_t)7;
    }
    seq_t res = (seq_t)2 * nseq;
    res += pow5;
    res += pow7;
    return res;
}

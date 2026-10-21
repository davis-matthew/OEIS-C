/*
 * A083315  a(n) = (2*5^n-(4^n-2^n))/2.
 * https://oeis.org/A083315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*5^n-(4^n-2^n))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t pow5 = pow_seq_t((seq_t)5, n);
    seq_t pow4 = pow_seq_t((seq_t)4, n);
    seq_t pow2 = pow_seq_t((seq_t)2, n);
    seq_t numerator = (seq_t)2 * pow5 - (pow4 - pow2);
    return numerator / (seq_t)2;
}

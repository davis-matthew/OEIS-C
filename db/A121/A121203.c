/*
 * A121203  a(n) = 2n+7^n-5^n.
 * https://oeis.org/A121203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 7^n - 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow7 = 1;
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow7 *= (seq_t)7;
        pow5 *= (seq_t)5;
    }
    seq_t two_n = ((seq_t)n) * (seq_t)2;
    return pow7 - pow5 + two_n;
}

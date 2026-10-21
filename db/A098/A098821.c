/*
 * A098821  a(n) = (n-2) * 2^(n-1) + 5.
 * https://oeis.org/A098821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*2^(n-1) + 5 for n >= 2; a(0) = a(1) = 4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)4;

    seq_t pow = ((seq_t)1) << (n - 1);
    seq_t res = ((seq_t)(n - 2)) * pow + (seq_t)5;
    return res;
}

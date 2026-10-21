/*
 * A066998  a(0)=0; a(n) = n^2*a(n-1) + 1.
 * https://oeis.org/A066998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*a(n-1) + 1; a(0)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = ((seq_t)i * (seq_t)i) * res + 1;
    }
    return res;
}

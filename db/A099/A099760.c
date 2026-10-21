/*
 * A099760  a(n+1) = 2*n*a(n) + 2 with a(0)=1.
 * https://oeis.org/A099760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n >= 0, a(n+1) = 2*n*a(n) + 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i) {
        result = ((seq_t)2 * (seq_t)i * result) + (seq_t)2;
    }
    return result;
}

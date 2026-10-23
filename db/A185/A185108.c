/*
 * A185108  a(0)=0; for n>0, a(n) = (n+2)*a(n-1) + 1.
 * https://oeis.org/A185108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, a(n) = (n+2)*a(n-1) + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t result = (seq_t)0; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        result = ((seq_t)i + (seq_t)2) * result + (seq_t)1;
    }
    return result;
}

/*
 * A145502  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 2.
 * https://oeis.org/A145502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; a(n+1)=a(n)^2+2*a(n)-2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, ignore n==0 */
    seq_t val = (seq_t)2;
    for (idx_t i = 1; i < n; ++i)
        val = val * val + ((seq_t)2) * val - ((seq_t)2);
    return val;
}

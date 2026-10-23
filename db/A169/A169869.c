/*
 * A169869  Maximum number of rational points on a smooth absolutely
 *          irreducible projective curve of genus n over the field F_2.
 * https://oeis.org/A169869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n+4 for 1 <= n <= 6; a(n) = n+3 for n >= 7
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6)
        return (seq_t)(n + 4);
    else
        return (seq_t)(n + 3);
}

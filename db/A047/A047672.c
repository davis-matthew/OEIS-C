/*
 * A047672  Row 3 of square array defined in A047671.
 * https://oeis.org/A047672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 3*n^2 + 4*n - 2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    return (seq_t)2 * n3 - (seq_t)3 * n2 + (seq_t)4 * sn - (seq_t)2;
}

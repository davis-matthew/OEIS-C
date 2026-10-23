/*
 * A157797  a(n) = 8984250*n - 1996170.
 * https://oeis.org/A157797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8984250*n - 1996170
 * Range:     u8 none, u16 none, u32 n <= 478, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t c1 = 8984250;
    const seq_t c2 = 1996170;
    return (seq_t)n * c1 - c2;
}

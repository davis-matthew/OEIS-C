/*
 * A181440  a(1) = 2; for n > 1, a(n) = A000217(n)-(sum of previous terms).
 * https://oeis.org/A181440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2; a(2) = 1; a(n) = n for all n ≥ 3
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)1;
    return (seq_t)n;
}

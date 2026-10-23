/*
 * A131439  Inverse binomial transform of A131438 (assuming zero offset in
 *          both sequences).
 * https://oeis.org/A131439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 7, and for n ≥ 3, a(n) = 4*n + 2 (which is
 *            equivalent to a(n) = 2*a(n-1) – a(n-2) for n>4)
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 7;
    return (seq_t)4 * n + 2;
}

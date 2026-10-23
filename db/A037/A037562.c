/*
 * A037562  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2, a(n) = 5·a(n-1) + d(n) where d(n) = 2 if n mod 3 ≠ 0,
 *            else 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not defined by the OEIS entry, return 0 for safety. */
    if (n == 0) return (seq_t)0;

    seq_t value = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* d(i) = 2 unless i is a multiple of 3, then d(i) = 1. */
        seq_t d = ((i % 3) == 0) ? (seq_t)1 : (seq_t)2;
        value = value * (seq_t)5 + d;
    }
    return value;
}

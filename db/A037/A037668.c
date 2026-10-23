/*
 * A037668  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i=0}^{n-1} period[i mod 3] * 5^{n-1-i}, where period
 *            = {3, 2, 0}. This is equivalent to a(0) = 0, a(k) = 5*a(k-1) +
 *            period[(k-1) % 3] for k ≥ 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[3] = {3, 2, 0};
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
        result = result * 5 + period[i % 3];
    return result;
}

/*
 * A218736  a(n) = (33^n - 1)/32.
 * https://oeis.org/A218736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (33^n - 1)/32 Recurrence: a(0) = 0, a(n) = 33*a(n-1) + 1
 *            (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
        result = result * (seq_t)33 + (seq_t)1;
    return result;
}

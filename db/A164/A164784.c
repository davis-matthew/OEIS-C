/*
 * A164784  a(n) = 6^n-5.
 * https://oeis.org/A164784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 5 Recurrence: a(1) = 1; a(n) = 6*a(n-1) + 25 for n
 *            > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
        res *= (seq_t)6;
    return res - (seq_t)5;
}

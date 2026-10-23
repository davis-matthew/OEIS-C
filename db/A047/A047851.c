/*
 * A047851  a(n) = A047848(3,n).
 * https://oeis.org/A047851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (6^n + 4)/5 Recurrence: a(0) = 1; a(n) = 6*a(n-1) - 4 (n
 *            >= 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 6 - 4;
    return val;
}

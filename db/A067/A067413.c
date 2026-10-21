/*
 * A067413  Sixth column of triangle A067410.
 * https://oeis.org/A067413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7·12^(n-1) for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* multiply by 12 exactly n‑1 times */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)12;

    return result * (seq_t)7;
}

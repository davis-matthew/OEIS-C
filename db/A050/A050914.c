/*
 * A050914  a(n) = n*3^n + 1.
 * https://oeis.org/A050914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*3^n + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i)
        power *= 3;
    return power * (seq_t)n + 1;
}

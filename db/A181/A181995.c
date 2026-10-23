/*
 * A181995  a(n) = if n mod 2 = 1 then n*(n - 1) else (n - 1)^2 + (n - 2)/2.
 * https://oeis.org/A181995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 2 = 1 then n*(n - 1) else (n - 1)^2 + (n - 2)/2
 * Range:     u8 n <= 16, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return 0;
    if (n & 1U) /* odd */
        return (seq_t)n * (seq_t)(n - 1U);
    else {       /* even */
        seq_t n1 = (seq_t)(n - 1U);
        return n1 * n1 + (seq_t)((n - 2U) / 2U);
    }
}

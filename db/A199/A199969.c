/*
 * A199969  a(n) = the greatest non-divisor h of n (1 < h < n), or 0 if no
 *          such h exists.
 * https://oeis.org/A199969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 0 if n < 3, otherwise a(n) = n-1 (greatest non‑divisor of
 *            n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    return (seq_t)n - 1;
}

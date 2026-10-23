/*
 * A036407  a(n) = ceiling(n^2/9).
 * https://oeis.org/A036407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/9)
 * Range:     u8 n <= 47, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 64‑bit arithmetic is sufficient for all required n
       (up to at least 10 000) and works for any seq_t
       that can hold the result.  The calculation is
       performed exactly in uint64_t and then converted
       to seq_t – truncation is acceptable when the
       value does not fit. */
    unsigned long long sq   = (unsigned long long)n * n;    /* n^2          */
    unsigned long long res  = (sq + 8) / 9;                 /* ceil(n^2/9)  */
    return (seq_t)res;
}

/*
 * A105338  a(n) = n*(n+1)/2 mod 512.
 * https://oeis.org/A105338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 mod 512
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 64‑bit intermediate to avoid overflow for any reasonable n */
    unsigned long long nn = n;
    unsigned long long val = nn * (nn + 1ULL) / 2ULL;
    return (seq_t)(val & 511ULL);   /* 512 = 2^9, so mask with 511 */
}

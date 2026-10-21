/*
 * A086942  Integers k such that R(k+8) = 4.
 * https://oeis.org/A086942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*10^n - 8 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; for any n≥1 compute 4*10^n - 8 */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= 10;           /* pow10 = 10^n  (within seq_t) */

    seq_t result = pow10 * 4;   /* 4*10^n   */
    result -= 8;                /* -8        */

    return result;
}

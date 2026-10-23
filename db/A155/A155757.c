/*
 * A155757  a(n) = (n^3 - n + 15)/3.
 * https://oeis.org/A155757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n + 15)/3
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 2344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) /* undefined for n < 1, but guard against underflow */
        return (seq_t)0;
    /* Compute the numerator in 64‑bit unsigned arithmetic (safe up to n≈2¹⁶) */
    idx_t nn = n;
    idx_t num = nn * nn * nn - nn + 15;
    /* Division is exact for all integer n */
    num /= 3;
    return (seq_t)num;
}

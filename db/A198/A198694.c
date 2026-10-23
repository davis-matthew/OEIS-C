/*
 * A198694  a(n) = 7*4^n-1.
 * https://oeis.org/A198694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*4^n-1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 6;               /* a(0) */
    for (idx_t i = 0; i < n; ++i)
        val = val * 4 + 3;       /* a(k) = 4*a(k‑1) + 3 */
    return val;
}

/*
 * A155095  Numbers k such that k^2 == -1 (mod 17).
 * https://oeis.org/A155095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34*n + (-1)^n - 17)/4 (from OEIS A155095)
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 (offset is 1) but allow guard for n == 0 */
    if (n == 0) return 0u;

    seq_t nn   = (seq_t)n;          /* convert index to seq_t */
    seq_t num  = nn * 34u;          /* 34*n  */
    if ((n & 1u) == 0u)            /* even n: add 1, odd n: subtract 1 */
        num += 1u;
    else
        num -= 1u;
    num -= 17u;                     /* subtract 17 */
    return num / 4u;                /* divide by 4 */
}

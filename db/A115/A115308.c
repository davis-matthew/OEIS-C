/*
 * A115308  a(n) = if n<8 then n else 8*a(floor(n/8)) + 7 - n mod 8.
 * https://oeis.org/A115308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n<8 then n else 8*a(floor(n/8)) + 7 - (n mod 8)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8u)          /* base case: a(n) = n for 1 ≤ n < 8 */
        return (seq_t)n;

    /* recursive step: n = 8*m + r with 0 ≤ r < 8 */
    idx_t m   = n >> 3;           /* floor(n/8) */
    seq_t sub = a(m);             /* a(floor(n/8)) */
    seq_t rem = (seq_t)(7u - (n & 7u)); /* 7 - n mod 8 */

    return 8u * sub + rem;
}

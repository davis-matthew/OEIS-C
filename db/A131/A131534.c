/*
 * A131534  Period 3: repeat [1, 2, 1].
 * https://oeis.org/A131534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - ((n+2)^2 % 3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We only need n modulo 3. */
    idx_t r = (n + 2) % 3;          /* r in {0,1,2} */
    idx_t sq = (r * r) % 3;         /* square modulo 3 (0,1,1) */
    return (seq_t)(2 - sq);         /* yields 1 or 2 */
}

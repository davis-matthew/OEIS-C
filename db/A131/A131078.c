/*
 * A131078  Periodic sequence (1, 1, 1, 1, 0, 0, 0, 0).
 * https://oeis.org/A131078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if ((n-1) mod 8) < 4, otherwise 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: n starts at 1 */
    /* Period 8: first 4 terms are 1, next 4 are 0 */
    idx_t mod8 = (n - 1) & 7ULL;      /* same as (n-1) % 8 for power of two period */
    return (mod8 < 4) ? (seq_t)1 : (seq_t)0;
}

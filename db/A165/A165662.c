/*
 * A165662  Period 5: repeat 4,4,8,6,8.
 * https://oeis.org/A165662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (2*n^2 + 8*n + 4) mod 10
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-5) (period 5). */

seq_t a(idx_t n)
{
    /* Period‑5 pattern: 4, 4, 8, 6, 8. */
    static const seq_t pat[5] = {4U, 4U, 8U, 6U, 8U};
    return pat[(unsigned)(n % 5)];
}

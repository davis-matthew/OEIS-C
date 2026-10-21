/*
 * A230239  Values of N for which the equation x^2 - 4*y^2 = N has integer
 *          solutions.
 * https://oeis.org/A230239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((4*(n-1)+1)/7) + floor((n-1+3)/7) + (n-1)
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; compute m = n-1 (0‑based) */
    idx_t m = n - 1;
    seq_t mm = (seq_t)m;

    /* integer division truncates toward zero for positive numbers */
    seq_t part1 = (4 * mm + 1) / 7;   /* floor((4m + 1)/7) */
    seq_t part2 = (mm + 3) / 7;       /* floor((m + 3)/7) */

    return mm + 2 * part1 + part2;
}

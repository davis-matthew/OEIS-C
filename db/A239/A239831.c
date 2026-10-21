/*
 * A239831  Floor(7n^2/2) + floor(5n/2) + floor(3n/7).
 * https://oeis.org/A239831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*n^2/2) + floor(5*n/2) + floor(3*n/7)
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the arithmetic type first */
    seq_t x = (seq_t)n;

    /* compute each floor term with integer division */
    seq_t t1 = (7 * x * x) / 2;   /* floor(7n^2/2) */
    seq_t t2 = (5 * x) / 2;       /* floor(5n/2)   */
    seq_t t3 = (3 * x) / 7;       /* floor(3n/7)   */

    return t1 + t2 + t3;
}

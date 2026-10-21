/*
 * A066164  Sum of interior angles in an n-sided polygon in degrees.
 * https://oeis.org/A066164
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*180
 * Range:     u8 n <= 3, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0; /* behaviour below offset is undefined */
    seq_t x = (seq_t)n;  /* cast index to seq_t */
    x -= 2;              /* subtract 2 */
    x *= 180;            /* multiply by 180 */
    return x;
}

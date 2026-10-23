/*
 * A183919  Characteristic sequence for sin(2*Pi/n) being rational.
 * https://oeis.org/A183919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if sin(2*Pi/n) is rational, 0 otherwise.
   For integer n, this occurs only when n is 1, 2, 4 or 12. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    return (n == 1 || n == 2 || n == 4 || n == 12) ? 1 : 0;
}

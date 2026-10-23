/*
 * A193608  The consecutive squares of numbers multiplied by their next
 *          consecutive integer.
 * https://oeis.org/A193608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(n^2 + (n+1)^2)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1289, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the target integer type */
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    seq_t n2 = nn + 2;

    /* n^2 + (n+1)^2 */
    seq_t term = nn * nn + n1 * n1;

    /* final result (n+2) * term */
    return n2 * term;
}

/*
 * A158067  a(n) = 64*n^2 - 2*n.
 * https://oeis.org/A158067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 - 2*n
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8192, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the offset is 1, but we return 0 anyway. */
    if (n == 0)
        return (seq_t)0;

    seq_t tn = (seq_t)n;
    seq_t val = 64 * tn * tn - 2 * tn;
    return val;
}

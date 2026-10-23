/*
 * A197903  a(n) = ceiling((n+1/n)^4).
 * https://oeis.org/A197903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n+1/n)^4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1,2 the ceiling gives special values.  For n ≥ 3 the
       expression simplifies to n^4 + 4*n^2 + 7. */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)40;

    seq_t nn = (seq_t)n * (seq_t)n;      /* n^2   */
    seq_t n4 = nn * nn;                  /* n^4   */
    return n4 + (seq_t)4 * nn + (seq_t)7;
}

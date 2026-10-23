/*
 * A102296  a(n) = (1/6)*(n+1)*(10*n^2 + 17*n + 12).
 * https://oeis.org/A102296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(10*n^2 + 17*n + 12)/6
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1370, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t a_term = nn + 1;
    seq_t tn2   = nn * nn;
    seq_t b_term = (seq_t)10 * tn2 + (seq_t)17 * nn + (seq_t)12;

    /* divide by 2 */
    if ((a_term & 1ULL) == 0)
        a_term >>= 1;
    else
        b_term >>= 1;

    /* divide by 3 */
    if (a_term % 3 == 0)
        a_term /= 3;
    else
        b_term /= 3;

    return a_term * b_term;
}

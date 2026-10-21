/*
 * A227039  Positive solutions x/(2^2*3*89) of the Pell equation x^2 - 73*y^2
 *          = -1.
 * https://oeis.org/A227039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4562498*a(n-1) - a(n-2) (n >= 2) a(0) = 1 a(1) = 4562499
 *            Generates the positive solutions x/(2^2*3*89) of the Pell
 *            equation x^2 - 73*y^2 = -1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ull)
        return (seq_t)1;
    if (n == 1ull)
        return (seq_t)4562499;

    seq_t prevprev = (seq_t)1;      /* a(0) */
    seq_t prev     = (seq_t)4562499;/* a(1) */
    seq_t cur;

    for (idx_t i = 2ull; i <= n; ++i) {
        cur = prev * (seq_t)4562498; /* 4562498 * a(i-1) */
        cur -= prevprev;            /* minus a(i-2) */
        prevprev = prev;
        prev = cur;
    }
    return prev;
}

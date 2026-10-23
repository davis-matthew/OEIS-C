/*
 * A033116  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) + 1 with a(1) = 1, a(2) = 6 (a(0)
 *            undefined, behavior for n<1 is unspecified)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* placeholder for n < 1, undefined per offset */

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)6;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 5 * prev1 + 6 * prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

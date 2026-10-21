/*
 * A208044  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to any horizontal, vertical or
 *          antidiagonal neighbor (colorings ignoring permutations of colors).
 * https://oeis.org/A208044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2) for n > 3 * Initial values: a(1)=2,
 *            a(2)=8, a(3)=44
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)44;

    /* Store a(n-2) and a(n-1) for the recurrence */
    seq_t prev2 = (seq_t)8;   /* a(2) */
    seq_t prev1 = (seq_t)44;  /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)7 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

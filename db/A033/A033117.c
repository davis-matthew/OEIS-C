/*
 * A033117  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-2) - 7*a(n-3) initial values: a(1) = 1
 *            a(2) = 7 a(3) = 50
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)50;

    /* a_{n-3}, a_{n-2}, a_{n-1} */
    seq_t prev3 = (seq_t)1;      /* a(1) */
    seq_t prev2 = (seq_t)7;      /* a(2) */
    seq_t prev1 = (seq_t)50;     /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur      = 7 * prev1 + prev2 - 7 * prev3;  /* a(n) */
        prev3 = prev2;  /* shift window */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

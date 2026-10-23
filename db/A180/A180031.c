/*
 * A180031  Number of n-move paths on a 3 X 3 chessboard of a queen starting
 *          or ending in the central square.
 * https://oeis.org/A180031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 8, a(n) = 5*a(n-1) + 8*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)8;

    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)8; /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2ULL; i <= n; ++i)
    {
        a_cur = a_prev1 * (seq_t)5 + a_prev2 * (seq_t)8;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}

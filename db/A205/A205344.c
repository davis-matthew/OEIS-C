/*
 * A205344  Number of length 7 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements unequal but differing by no more
 *          than n.
 * https://oeis.org/A205344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (176*n^5 - 25*n^4 + 25*n^2 + 124*n)/60
 * Range:     u8 none, u16 n <= 3, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(0) is unused but we return 0 for safety. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;        /* n */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n4 = n2 * n2;         /* n^4 */
    seq_t n5 = n4 * nn;         /* n^5 */

    seq_t term = 176 * n5;      /* 176*n^5 */
    term = term - 25 * n4;      /* 176*n^5 - 25*n^4 */
    term = term + 25 * n2;      /* + 25*n^2 */
    term = term + 124 * nn;     /* + 124*n */

    return term / 60;           /* divide by 60 (the common denominator) */
}

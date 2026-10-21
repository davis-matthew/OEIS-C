/*
 * A094895  Sequence generated from a Knight's tour of a 4 X 4 chessboard
 *          considered as a matrix.
 * https://oeis.org/A094895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=280, a(3)=8524; for n ≥ 4, a(n) = 24*a(n-1) +
 *            324*a(n-2) + 544*a(n-3) (linear recurrence of order 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not defined for n<1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)280;
    if (n == 3) return (seq_t)8524;

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)1;     /* a(1)   */
    seq_t a2 = (seq_t)280;   /* a(2)   */
    seq_t a3 = (seq_t)8524;  /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)24 * a3 + (seq_t)324 * a2 + (seq_t)544 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

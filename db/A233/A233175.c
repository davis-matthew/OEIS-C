/*
 * A233175  Number of 2 X n 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabelled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 128*a(n-3) for n>4; initial values: a(1)=3,
 *            a(2)=8, a(3)=80, a(4)=800
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)80;
    if (n == 4) return (seq_t)800;

    /* For n > 4 use the recurrence. */
    /* prev1 = a(n-1), prev2 = a(n-2), prev3 = a(n-3). */
    seq_t prev1 = (seq_t)800;   /* a(4)   */
    seq_t prev2 = (seq_t)80;    /* a(3)   */
    seq_t prev3 = (seq_t)8;     /* a(2)   */
    seq_t i, an;

    for (i = 5; i <= n; ++i) {
        an = (seq_t)12 * prev1 - (seq_t)128 * prev3;
        /* Shift variables for next step. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }
    return an;
}

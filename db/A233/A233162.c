/*
 * A233162  Number of n X 1 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabeled 8-color...
 * https://oeis.org/A233162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 44*a(n-2) + 48*a(n-3) for n>4, with a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=11
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Base cases */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;

    /* Sliding window for the recurrence */
    seq_t a_nm3 = (seq_t)1;   /* a(1)   */
    seq_t a_nm2 = (seq_t)1;   /* a(2)   */
    seq_t a_nm1 = (seq_t)3;   /* a(3)   */
    seq_t a_i   = (seq_t)0;   /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        if (i == 4) {
            a_i = (seq_t)11;   /* a(4) given explicitly */
        } else {
            a_i = (seq_t)12 * a_nm1
                  - (seq_t)44 * a_nm2
                  + (seq_t)48 * a_nm3;
        }
        /* Shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_i;
}

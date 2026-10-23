/*
 * A122365  The (1,6)-entry of the matrix M^n, where M is the 6 X 6 matrix
 *          {{1, 1, 1, 1, 1, 1},{1, 0, 0, 0, 1, 0},{1, 0, 0, 1, 0, 0},{1, 0,
 *          1, 0, 0, 0},{1, 1, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}}.
 * https://oeis.org/A122365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - a(n-3), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* variables to hold the last three values of the recurrence */
    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)1;   /* a(n-1) */
    seq_t next;

    /* iterate from 3 up to n, updating the sliding window */
    for (idx_t i = 3; i <= n; ++i) {
        next = 2 * a2 + 4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;   /* a(n) */
}

/*
 * A240784  Number of 3 X n 0..1 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A240784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) - a(n-5), a(1) = 4, a(2) = 11,
 *            a(3) = 20, a(4) = 46, a(5) = 97
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)46;
    if (n == 5) return (seq_t)97;

    /* Store the last five values during the iterative computation.
       indices 1..5 correspond to a(i-5) .. a(i-1) for the next step. */
    seq_t a[6] = {0};              /* a[0] unused */
    a[1] = (seq_t)4;  /* a(1)   */
    a[2] = (seq_t)11; /* a(2)   */
    a[3] = (seq_t)20; /* a(3)   */
    a[4] = (seq_t)46; /* a(4)   */
    a[5] = (seq_t)97; /* a(5)   */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * a[5] + a[4] - a[3] - a[1];
        /* shift the window */
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = a[5];
        a[5] = next;
    }

    return a[5];
}

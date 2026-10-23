/*
 * A132343  Output of Knuth's "man or boy" test for varying k.
 * https://oeis.org/A132343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-5) - 6*a(n-4) + 11*a(n-3) - 10*a(n-2) + 5*a(n-1);
 *            Initial values: a(0)=1, a(1)=0, a(2)=-2, a(3)=0, a(4)=1, a(5)=0,
 *            a(6)=1, a(7)=-1, a(8)=-10, a(9)=-30
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 31, i64 n <= 59, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {1, 0, -2, 0, 1, 0, 1, -1, -10, -30};

    if (n < 10)
        return init[n];

    /* Buffer holds the last five terms: a[i-5] .. a[i-1] during iteration. */
    seq_t buf[5];
    buf[0] = init[5];  /* a(5)   */
    buf[1] = init[6];  /* a(6)   */
    buf[2] = init[7];  /* a(7)   */
    buf[3] = init[8];  /* a(8)   */
    buf[4] = init[9];  /* a(9)   */

    for (idx_t i = 10; i <= n; ++i) {
        seq_t next =
            buf[0] - 6 * buf[1] + 11 * buf[2] - 10 * buf[3] + 5 * buf[4];

        /* Shift buffer left: a[i-4] .. a[i] will be in buf after shift. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];  /* a(n) */
}

/*
 * A098554  G.f.: x*(1-x^2)/((1+x^2)*(1+x+x^2)).
 * https://oeis.org/A098554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - a(n-3) - a(n-4) (n ≥ 4) * initial
 *            values: * a(0) = 0, a(1) = 1, a(2) = -1, a(3) = -2
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)-2;

    /* ring buffer storing the last four terms:
     * last[0] = a(n-4), last[1] = a(n-3),
     * last[2] = a(n-2), last[3] = a(n-1)   */
    seq_t last[4];
    last[0] = (seq_t)0;   /* a(0)   */
    last[1] = (seq_t)1;   /* a(1)   */
    last[2] = (seq_t)-1;  /* a(2)   */
    last[3] = (seq_t)-2;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = -( last[3] + (seq_t)2 * last[2] + last[1] + last[0] );
        /* shift the buffer: a(n-4) <- a(n-3), a(n-3) <- a(n-2),
         * a(n-2) <- a(n-1), a(n-1) <- next                */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = next;
    }
    return last[3];
}

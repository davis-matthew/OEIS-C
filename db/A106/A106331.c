/*
 * A106331  Numbers j such that 24*(j^2) + 25 = k^2.
 * https://oeis.org/A106331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=1, a(3)=2, a(4)=5, a(5)=12, a(6)=21;
 * For n>6:  a(n) = 10*a(n-3) - a(n-6)                  */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    /* Base values for n = 1 … 6 */
    if (n <= 6) {
        static const seq_t base[7] = {0, 0, 1, 2, 5, 12, 21};
        return base[n];
    }

    /* Keep the last six values a[n-6] … a[n-1] */
    seq_t v[6];
    v[0] = (seq_t)0;   /* a(1)   */
    v[1] = (seq_t)1;   /* a(2)   */
    v[2] = (seq_t)2;   /* a(3)   */
    v[3] = (seq_t)5;   /* a(4)   */
    v[4] = (seq_t)12;  /* a(5)   */
    v[5] = (seq_t)21;  /* a(6)   */

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 10*a(i-3) - a(i-6) */
        seq_t next = (seq_t)10 * v[3] - v[0];

        /* shift left: a(i-5)..a(i-1) become u[0]..u[4] */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];   /* a(n) */
}

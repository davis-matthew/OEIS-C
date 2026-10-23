/*
 * A102301  a(n) = ((3*n + 1)*2^(n+3) + 9 + (-1)^n)/18.
 * https://oeis.org/A102301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3*n + 1)*2^(n+3) + 9 + (-1)^n)/18 Recurrence derived
 *            from generating function 1/((1-x^2)*(1-2x)^2): a(0)=1, a(1)=4,
 *            a(2)=13, a(3)=36 a(n) = 4*a(n-1) - 3*a(n-2) - 4*a(n-3) +
 *            4*a(n-4) for n >= 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[4] = { (seq_t)1, (seq_t)4, (seq_t)13, (seq_t)36 };

    if (n < 4)
        return init[n];

    /* slide window of the last four terms: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_n4 = init[0];  /* a(n-4) */
    seq_t a_n3 = init[1];  /* a(n-3) */
    seq_t a_n2 = init[2];  /* a(n-2) */
    seq_t a_n1 = init[3];  /* a(n-1) */
    seq_t a_n  = 0;        /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)4 * a_n1
            - (seq_t)3 * a_n2
            - (seq_t)4 * a_n3
            + (seq_t)4 * a_n4;

        /* shift window */
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = a_n;
    }
    return a_n;
}

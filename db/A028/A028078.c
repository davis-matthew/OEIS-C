/*
 * A028078  Expansion of 1/((1-3*x)*(1-6*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A028078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*12^(n+3)-27*7^(n+3)+30*6^(n+3)-5*3^(n+3))/540
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 28*a(n-1)-273*a(n-2)+1098*a(n-3)-1512*a(n-4) */

seq_t a(idx_t n)
{
    /* initial terms a(0) .. a(3) */
    const seq_t a0_0 = (seq_t)1;          /* a(0) */
    const seq_t a0_1 = (seq_t)28;         /* a(1) */
    const seq_t a0_2 = (seq_t)511;        /* a(2) */
    const seq_t a0_3 = (seq_t)7762;       /* a(3) */

    if (n == 0) return a0_0;
    if (n == 1) return a0_1;
    if (n == 2) return a0_2;
    if (n == 3) return a0_3;

    /* variables hold a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a_n_minus_4 = a0_0;
    seq_t a_n_minus_3 = a0_1;
    seq_t a_n_minus_2 = a0_2;
    seq_t a_n_minus_1 = a0_3;
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(n) using the recurrence */
        a_n = 28 * a_n_minus_1
            - 273 * a_n_minus_2
            + 1098 * a_n_minus_3
            - 1512 * a_n_minus_4;

        /* Shift variables for next iteration */
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}

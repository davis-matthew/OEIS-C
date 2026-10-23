/*
 * A128183  Row sums of triangle A128182.
 * https://oeis.org/A128183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*floor(k/2+1)^2 Recurrence
 *            (from the generating function): a(0)=1, a(1)=2, a(2)=7, a(3)=20,
 *            a(4)=54, a(5)=140 For n ≥ 6: a(n) = 6*a(n-1) - 12*a(n-2) +
 *            8*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed initial values up to n = 5 */
    static const seq_t init[6] = {
        (seq_t)1,   /* a(0) */
        (seq_t)2,   /* a(1) */
        (seq_t)7,   /* a(2) */
        (seq_t)20,  /* a(3) */
        (seq_t)54,  /* a(4) */
        (seq_t)140  /* a(5) */
    };

    if (n < 6) {
        /* n fits into the pre‑computed table */
        return init[n];
    }

    /* Initialise the last three terms a(3), a(4), a(5) */
    seq_t a_n_minus_3 = init[3];   /* a(3) */
    seq_t a_n_minus_2 = init[4];   /* a(4) */
    seq_t a_n_minus_1 = init[5];   /* a(5) */
    seq_t a_n;                     /* current term */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 12*a(i-2) + 8*a(i-3) */
        a_n = (seq_t)6 * a_n_minus_1
            - (seq_t)12 * a_n_minus_2
            + (seq_t)8  * a_n_minus_3;

        /* shift the window for next iteration */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}

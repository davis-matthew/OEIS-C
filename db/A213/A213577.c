/*
 * A213577  Principal diagonal of the convolution array A213576.
 * https://oeis.org/A213577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 3*a(n-3) + 6*a(n-4) + a(n-5) -
 *            a(n-6) Initial values (n = 1…6): a(1) = 1, a(2) = 4, a(3) = 17,
 *            a(4) = 56, a(5) = 172, a(6) = 498
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms a(1) .. a(6) */
    static const seq_t init[6] = {1, 4, 17, 56, 172, 498};

    /* return immediately for the first six entries */
    if (n <= 6)
        return init[n - 1];

    /* sliding window for a(n-6) .. a(n-1) */
    seq_t a_nm6 = init[0];
    seq_t a_nm5 = init[1];
    seq_t a_nm4 = init[2];
    seq_t a_nm3 = init[3];
    seq_t a_nm2 = init[4];
    seq_t a_nm1 = init[5];
    seq_t a_n;

    for (idx_t i = 7; i <= n; ++i) {
        a_n = 5 * a_nm1 - 6 * a_nm2 - 3 * a_nm3 + 6 * a_nm4 + a_nm5 - a_nm6;

        /* shift the window for the next iteration */
        a_nm6 = a_nm5;
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

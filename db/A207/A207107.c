/*
 * A207107  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=5*a(n-1)-10*a(n-2)+10*a(n-3)-5*a(n-4)+a(n-5) for n>5;
 *            initial values: a(1)=9, a(2)=81, a(3)=271, a(4)=643, a(5)=1271
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; value not used for n<1 */
    static const seq_t init[5] = {9, 81, 271, 643, 1271};

    if (n <= 5)
        return init[n - 1];

    /* a_nm1 = a(i-1), a_nm2 = a(i-2), ..., a_nm5 = a(i-5) */
    seq_t a_nm1 = init[4];      /* a5  */
    seq_t a_nm2 = init[3];      /* a4  */
    seq_t a_nm3 = init[2];      /* a3  */
    seq_t a_nm4 = init[1];      /* a2  */
    seq_t a_nm5 = init[0];      /* a1  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t temp = 5 * a_nm1 + 10 * a_nm3 + a_nm5; /* 5*a(i-1)+10*a(i-3)+a(i-5) */
        temp -= 10 * a_nm2;                          /* -10*a(i-2) */
        temp -= 5 * a_nm4;                           /* -5*a(i-4) */

        /* Shift values for next step */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = temp;
    }
    return a_nm1;
}

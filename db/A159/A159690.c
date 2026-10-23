/*
 * A159690  Positive numbers y such that y^2 is of the form x^2+(x+881)^2 with
 *          integer x.
 * https://oeis.org/A159690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) – a(n-6) for n > 6 Initial values: a(1)=841,
 *            a(2)=881, a(3)=925, a(4)=4121, a(5)=4405, a(6)=4709
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial six terms */
    const seq_t init[6] = {841, 881, 925, 4121, 4405, 4709};

    if (n <= 6) {
        return init[n - 1];
    }

    /* Circular buffer holding the last six terms:
       buf[0] = a(i-6), buf[1] = a(i-5), ..., buf[5] = a(i-1) */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i) {
        buf[i] = init[i];
    }

    seq_t next = 0;  /* will hold a(i) as we iterate */

    for (idx_t i = 7; i <= n; ++i) {
        /* indices for a(i-3) and a(i-6) in the moving buffer */
        int idx_a_minus_3 = (int)((i - 4) % 6); /* (i-3)-1 */
        int idx_a_minus_6 = (int)((i - 7) % 6); /* (i-6)-1 */

        next = 6 * buf[idx_a_minus_3] - buf[idx_a_minus_6];
        buf[(int)((i - 1) % 6)] = next;  /* store a(i) in place of a(i-6) */
    }

    return next;
}

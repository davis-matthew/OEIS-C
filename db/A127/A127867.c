/*
 * A127867  Number of tilings of a 3 X n board with 1 X 1 and L-shaped tiles
 *          (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) - a(n-3) + 2*a(n-4) (n>=4) Initial
 *            values: a(0)=1, a(1)=1, a(2)=11, a(3)=39 (derived from the
 *            generating function (1-x)^2/(1-3x-7x^2+x^3-2x^4))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)11;
    if (n == 3)
        return (seq_t)39;

    /* Use a rolling window of the last four values to compute a(n). */
    seq_t a_nm4 = (seq_t)1;  /* a(n-4)  */
    seq_t a_nm3 = (seq_t)1;  /* a(n-3)  */
    seq_t a_nm2 = (seq_t)11; /* a(n-2)  */
    seq_t a_nm1 = (seq_t)39; /* a(n-1)  */
    seq_t a_n;               /* current term */

    /* Compute progressively up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        a_n = ((seq_t)3) * a_nm1
            + ((seq_t)7) * a_nm2
            - a_nm3
            + ((seq_t)2) * a_nm4;

        /* Slide the window for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

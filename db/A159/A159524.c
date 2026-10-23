/*
 * A159524  Numerator of Hermite(n, 7/16).
 * https://oeis.org/A159524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=7, a(n) = 7*a(n-1) - 128*(n-1)*a(n-2) (n >= 2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)7;   /* a(1) */
    seq_t a_n;                /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute 128*(i-1) as a seq_t to keep types uniform */
        seq_t mult = (seq_t)(i - 1) * (seq_t)128;
        /* Term a(i) = 7*a(i-1) - mult * a(i-2) */
        a_n = a_nm1 * (seq_t)7 - (a_nm2 * mult);
        /* Shift variables for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

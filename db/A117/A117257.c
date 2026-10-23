/*
 * A117257  Column 0 of triangle A117256.
 * https://oeis.org/A117257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [∏_{j=0}^{n-1} (1-5*j)]/n! * 5^(n*(n-1)/2)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val = 1;            /* a(0) */
    seq_t power5 = 1;           /* 5^(n-1) for current n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mul = 1 - (seq_t)5 * (i - 1);   /* (1-5*(i-1)) */
        a_val = a_val * mul;                 /* multiply by factor */
        a_val = a_val * power5;              /* multiply by 5^(i-1) */
        a_val = a_val / (seq_t)i;            /* divide by i (= n!) */
        power5 = power5 * (seq_t)5;          /* update 5^i for next step */
    }
    return a_val;
}

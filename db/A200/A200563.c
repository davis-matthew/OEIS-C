/*
 * A200563  Expansion of -2*x*(1+4*x) / ((2*x-1)*(4*x^2+3*x+1)).
 * https://oeis.org/A200563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 8*a(n-3)
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 31, i64 n <= 63, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1) = 2, a(2) = 6, a(3) = -2 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)6;
    if (n == 3)
        return (seq_t)(-2);

    /* Iteratively compute the sequence up to n using the recurrence. */
    seq_t a_nm3 = (seq_t)2;      /* a(1)    */
    seq_t a_nm2 = (seq_t)6;      /* a(2)    */
    seq_t a_nm1 = (seq_t)(-2);   /* a(3)    */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        a_n  = -a_nm1 + (seq_t)2 * a_nm2 + (seq_t)8 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

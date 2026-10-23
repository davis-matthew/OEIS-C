/*
 * A175714  Expansion of -1/((1 - x)*(1 - x^2 + 4*x^3)).
 * https://oeis.org/A175714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 39, i64 n <= 76, i128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from G.f.  -1/((1 - x)*(1 - x^2 + 4*x^3))
   a(0) = -1, a(1) = -1, a(2) = -2, a(3) = 2
   a(n) = a(n-1) + a(n-2) - 5*a(n-3) + 4*a(n-4)    for n ≥ 4 */
seq_t a(idx_t n)
{
    /* small indices are handled explicitly */
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(-1);
    if (n == 2) return (seq_t)(-2);
    if (n == 3) return (seq_t)( 2);

    /* recurrence uses only the last four values */
    seq_t a_nm4 = (seq_t)(-1);  /* a(n-4) */
    seq_t a_nm3 = (seq_t)(-1);  /* a(n-3) */
    seq_t a_nm2 = (seq_t)(-2);  /* a(n-2) */
    seq_t a_nm1 = (seq_t)( 2);  /* a(n-1) */
    seq_t a_n   = 0;            /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = a_nm1 + a_nm2 - (seq_t)5 * a_nm3 + (seq_t)4 * a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

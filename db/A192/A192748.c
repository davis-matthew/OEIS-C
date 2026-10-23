/*
 * A192748  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 0; a(2) = 1;
   for n >= 3:  a(n) = a(n-1) + a(n-2) + 3*(n-2);   (OEIS indexing)
   This is derived from the polynomial P(n,x) = x*P(n-1,x)+3n
   reduced by the relation x^2 = x+1, keeping only the linear term
   coefficient of x as the sequence term. */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)0;      /* a(1) */
    seq_t a_nm1 = (seq_t)1;      /* a(2) */
    seq_t a_n;                   /* a(n) while iterating */

    for (idx_t i = 3; i <= n; ++i) {
        /* a_n = a_{n-1} + a_{n-2} + 3*(n-2) */
        a_n = a_nm1 + a_nm2 + (seq_t)(3 * (i - 2));
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

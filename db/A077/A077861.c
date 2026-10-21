/*
 * A077861  Expansion of (1-x)^(-1)/(1-2*x+2*x^2+x^3).
 * https://oeis.org/A077861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1 + 2*a(n-1) - 2*a(n-2) - a(n-3),
   with a(0)=1, a(1)=3, a(2)=5.  The sequence is the
   coefficients of (1-x)^(-1)/(1-2x+2x^2+x^3). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)3;   /* a(1) */
    seq_t a_nm1 = (seq_t)5;   /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = ((seq_t)1) + ((seq_t)2) * a_nm1
                  - ((seq_t)2) * a_nm2
                  - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return a_nm1;
}

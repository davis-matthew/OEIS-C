/*
 * A220433  Expansion of e.g.f. for operads for alia algebras.
 * https://oeis.org/A220433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220433: Expansion of e.g.f. for operads for alia algebras
 * Initial values: a(0)=0, a(1)=1, a(2)=2
 * Recurrence for n≥3:
 *     4*a(n) + 6*(-2*n+3)*a(n-1) -(3*n-5)*(3*n-7)*a(n-2)=0
 * which is equivalent to
 *     a(n) = ((12*n-18)*a(n-1)+(3*n-5)*(3*n-7)*a(n-2))/4
 */
seq_t a(idx_t n)
{
    /* Handle the first three values directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* a_{n-2} = a(1) , a_{n-1} = a(2) initially */
    seq_t a_nm2 = 1;  /* a(n-2) */
    seq_t a_nm1 = 2;  /* a(n-1) */
    seq_t a_cur  = 0; /* current a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute the terms of the recurrence. */
        seq_t term1 = ((seq_t)12 * i - (seq_t)18) * a_nm1;
        seq_t f1    = (seq_t)3 * i - (seq_t)5;
        seq_t f2    = (seq_t)3 * i - (seq_t)7;
        seq_t term2 = f1 * f2 * a_nm2;

        /* a(n) = (term1 + term2) / 4  */
        a_cur = (term1 + term2) / (seq_t)4;

        /* Shift the values for the next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}

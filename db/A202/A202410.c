/*
 * A202410  Inverse Lah transform of 1,2,3,...; e.g.f.
 *          exp(x/(x-1))*(2*x-1)/(x-1).
 * https://oeis.org/A202410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 12, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} (-1)^k * (n-k)! * binomial(n,n-k) * binomial(n-1,n-k) * (k+1)
 * Recurrence: a(0)=1, a(1)=-2, a(2)=-1
 * For n ≥ 2: a(n+1) = 2*n*(n-1)*(n-2)*a(n-2) - n*(5*n-8)*a(n-1) + (4*n-2)*a(n)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)-1;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)-2;  /* a(1) */
    seq_t a_n   = (seq_t)-1;  /* a(2) */

    for (idx_t curr = 2; curr < n; ++curr) {
        seq_t c1 = (seq_t)2 * (seq_t)curr * (seq_t)(curr - 1) * (seq_t)(curr - 2);
        seq_t c2 = (seq_t)curr * (((seq_t)5 * (seq_t)curr) - (seq_t)8);
        seq_t c3 = ((seq_t)4 * (seq_t)curr) - (seq_t)2;

        seq_t term1 = c1 * a_nm2;
        seq_t term2 = c2 * a_nm1;
        seq_t term3 = c3 * a_n;

        seq_t next = term1 - term2 + term3;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
        a_n   = next;
    }

    return a_n;
}

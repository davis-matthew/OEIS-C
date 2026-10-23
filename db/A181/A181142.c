/*
 * A181142  Number of permutations of {1,2,...,2n} , say x(1),x(2), ... ,
 *          x(2n) , such that x(i) + x(i+1) is not equal to 2n-1 for all i,
 *          1<=i<=2n-1.
 * https://oeis.org/A181142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} C(n-1,k) * (-2)^k * (2n-k)! Recurrence (for
 *            n ≥ 3): (n‑1)*a(n) = 2*(2n‑1)*(n²‑n‑1)*a(n‑1) +
 *            4*n²*(n‑2)*a(n‑2) Initial values: a(1) = 2, a(2) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n = 0, return 0 */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)12;

    seq_t a_nm2 = (seq_t)2;   /* a(1)   */
    seq_t a_nm1 = (seq_t)12;  /* a(2)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t i_seq   = (seq_t)i;
        seq_t i_sq    = i_seq * i_seq;                /* i²  */
        seq_t coeff1  = (seq_t)2 * ((seq_t)2 * i_seq - 1) * (i_sq - i_seq - 1);
        seq_t coeff2  = (seq_t)4 * i_sq * (i_seq - 2);

        seq_t numerator = coeff1 * a_nm1 + coeff2 * a_nm2;
        seq_t denom     = i_seq - 1;

        seq_t a_n = numerator / denom;  /* exact division */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

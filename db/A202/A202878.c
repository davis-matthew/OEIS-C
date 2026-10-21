/*
 * A202878  Expansion of e.g.f.: exp(16*x/(1-x)) / sqrt(1-x^2).
 * https://oeis.org/A202878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+15)*a(n-1) + (n-1)*(n+15)*a(n-2) -
 *            (n-1)*(n-2)^2*a(n-3) with initial values a(0)=1, a(1)=16,
 *            a(2)=289. /* This function computes the sequence in O(n) time
 *            without external state
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)289;

    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)16;  /* a(1) */
    seq_t a_nm1 = (seq_t)289; /* a(2) */
    seq_t ai = a_nm1;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(i + 15) * a_nm1;
        seq_t term2 = (seq_t)(i - 1) * (seq_t)(i + 15) * a_nm2;
        seq_t term3 = (seq_t)(i - 1) *
                      ((seq_t)(i - 2) * (seq_t)(i - 2)) *
                      a_nm3;
        ai = term1 + term2 - term3;

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return ai;
}

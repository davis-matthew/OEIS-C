/*
 * A076035  Expansion of g.f. 1/(1-4*x*C) where C = (1/2-1/2*(1-4*x)^(1/2))/x
 *          = g.f. for Catalan numbers A000108.
 * https://oeis.org/A076035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*a(n-1) - 4*Catalan(n-1))/3, with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t a_prev = (seq_t)1;               /* a(0) */
    seq_t a_curr = (seq_t)0;
    seq_t cat    = (seq_t)1;               /* Catalan(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = (16*a(i-1) - 4*Catalan(i-1))/3 */
        seq_t temp   = (seq_t)16 * a_prev - (seq_t)4 * cat;
        a_curr = temp / (seq_t)3;

        /* Update Catalan number to Catalan(i):
           C_i = C_{i-1} * 2*(2*i-1)/(i+1) */
        seq_t mult = (seq_t)2 * ((seq_t)2 * i - (seq_t)1);
        cat = cat * mult / ((seq_t)i + (seq_t)1);

        a_prev = a_curr;
    }
    return a_curr;
}

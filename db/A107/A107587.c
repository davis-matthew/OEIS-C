/*
 * A107587  Number of Motzkin n-paths with an even number of up steps.
 * https://oeis.org/A107587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n, 2*k) * Catalan(k) (k even)
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;      /* C(n,0) */
    seq_t catalan = 1;    /* Catalan(0) */
    idx_t max_k = n / 2;  /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        if ((k & 1) == 0)           /* k even → add to the sum */
            sum += binom * catalan;

        if (k < max_k)              /* prepare for next k+1 */
        {
            /* current r = 2*k  */
            idx_t r = 2 * k;

            /* binomial recurrence: C(n, r+2) = C(n, r) * (n-r)*(n-r-1)/((r+1)*(r+2)) */
            seq_t temp = binom;
            temp *= (seq_t)(n - r);
            temp *= (seq_t)(n - r - 1);
            seq_t denom = (seq_t)(r + 1) * (seq_t)(r + 2);
            binom = temp / denom;

            /* Catalan recurrence: Catalan(k+1) = Catalan(k) * 2*(2k+1)/(k+2) */
            seq_t next = catalan * (seq_t)(2 * (2 * k + 1)) / (seq_t)(k + 2);
            catalan = next;
        }
    }
    return sum;
}

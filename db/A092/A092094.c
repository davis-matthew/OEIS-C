/*
 * A092094  a(n) = Sum_{i=0,1,2,..; n-k*i >= -n} |n-k*i| for k=3.
 * https://oeis.org/A092094
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0}^{⌊2n/3⌋} |n - 3*i|
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    idx_t m1 = n / 3;          /* floor(n/3)   */
    idx_t k  = (2 * n) / 3;    /* floor(2n/3)  */

    seq_t tn      = (seq_t)n;
    seq_t count1  = (seq_t)(m1 + 1);
    seq_t sum_i1  = (seq_t)m1 * (seq_t)(m1 + 1) / 2;      /* Σ i   i=0..m1 */
    seq_t A = count1 * tn - 3 * sum_i1;

    seq_t B = 0;
    if (k > m1) {
        idx_t count2 = k - m1;
        seq_t sum_i2 = ((seq_t)(m1 + 1) + (seq_t)k) * (seq_t)count2 / 2; /* Σ i   i=m1+1..k */
        B = 3 * sum_i2 - (seq_t)count2 * tn;
    }
    return A + B;
}

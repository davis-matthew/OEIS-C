/*
 * A087981  Expansion of e.g.f.: exp(-2*x) / (1-x)^2.
 * https://oeis.org/A087981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1, a(1)=0, a(n+1)=n*(a(n)+2*a(n-1)) (for n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)0; /* a(1) */

    for (idx_t k = 1; k < n; ++k) {
        seq_t next = (seq_t)k * (a_prev1 + (seq_t)2 * a_prev2);
        a_prev2 = a_prev1;
        a_prev1 = next;
    }
    return a_prev1;          /* a(n) */
}

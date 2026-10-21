/*
 * A097820  Expansion of e.g.f. exp(2*x)/(1-4*x).
 * https://oeis.org/A097820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=4*n*a(n-1)+2^n (n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)1;   /* a(0) */
    seq_t pow2 = (seq_t)1;   /* keeps 2^i, start with 2^0 */
    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;                                 /* pow2 = 2^i */
        val = ((seq_t)4 * (seq_t)i * val) + pow2;    /* recurrence */
    }
    return val;
}

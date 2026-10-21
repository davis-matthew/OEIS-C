/*
 * A063209  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          41 ).
 * https://oeis.org/A063209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for n>1, a(n) = (20*n - 8 - 2*((n-1) mod 3)) / 3
 * Range:     u8 n <= 38, u16 n <= 9830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1)=3; a(0) is not used but return 0 for safety */
        return (n == 1) ? (seq_t)3 : (seq_t)0;
    }
    seq_t N   = (seq_t)n;
    seq_t mod = (seq_t)((n - 1) % 3ULL);      /* (n-1) mod 3 */
    seq_t res = (20 * N - 8 - 2 * mod) / 3;
    return res;
}

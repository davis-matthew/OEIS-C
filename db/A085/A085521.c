/*
 * A085521  a(n) = Product_{k=0..n} (2^(2k+1)+1).
 * https://oeis.org/A085521
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (2^(2k+1)+1) (with offset -1: a(-1) = 1, a(0)
 *            = 3, a(1) = 27, …)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < -1)
        return (seq_t)0;             /* out of range – undefined, return 0 */
    if (n == -1)
        return (seq_t)1;             /* first term */
    seq_t result = (seq_t)1;        /* product accumulator */
    seq_t pow4   = (seq_t)1;        /* holds 4^k, starting with k = 0 */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        /* term = 2 * 4^k + 1 */
        seq_t term = pow4;
        term *= (seq_t)2;
        term += (seq_t)1;

        result *= term;

        /* next 4^k */
        pow4 *= (seq_t)4;
    }
    return result;
}

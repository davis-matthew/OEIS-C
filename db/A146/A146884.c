/*
 * A146884  a(n) = 7*Sum_{k=0..n} 6^k.
 * https://oeis.org/A146884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 7*Σ_{k=0..n}6^k = (7/5)*(6^(n+1) – 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t term = 1;

    for (idx_t i = 0; i <= n; ++i) {
        sum  += term;            /* accumulate Σ6^k   */
        term *= (seq_t)6;        /* next power of 6    */
    }
    return ((seq_t)7) * sum;     /* multiply by 7      */
}

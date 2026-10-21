/*
 * A097816  E.g.f. exp(5x)/(1-5x).
 * https://oeis.org/A097816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 5 * n * a(n-1) + 5^n (n > 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1, 5^0 = 1 */
    seq_t res   = 1;    /* current a(n)   */
    seq_t pow5  = 1;    /* 5^current i   */

    for (idx_t i = 1; i <= n; ++i) {
        /* update 5^i */
        pow5 *= (seq_t)5;

        /* res = 5 * i * res + pow5  */
        seq_t t = (seq_t)i * res;
        t *= (seq_t)5;
        res = t + pow5;
    }

    return res;
}

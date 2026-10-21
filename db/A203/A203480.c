/*
 * A203480  a(n) = v(n+1)/v(n), where v = A203479.
 * https://oeis.org/A203480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (2^k + 2^(n+1) - 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute t = 2^(n+1) - 2 */
    seq_t t = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i)
        t *= (seq_t)2;
    t -= (seq_t)2;

    seq_t ak  = (seq_t)2;   /* 2^1 */
    seq_t res = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = ak + t;
        res *= term;
        ak *= (seq_t)2;   /* next power of 2 */
    }

    return res;
}

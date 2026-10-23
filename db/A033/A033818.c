/*
 * A033818  Convolution of natural numbers n >= 1 with Lucas numbers L(k) for
 *          k >= -2.
 * https://oeis.org/A033818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(n+1) + n - 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Lucas(n+1) iteratively. */
    seq_t L0 = 2;                 /* Lucas(0) */
    seq_t L1 = 1;                 /* Lucas(1) */
    if ((n + 1) == 0)
        return L0 + (seq_t)n - 1; /* (never reached for n >= 1) */
    if ((n + 1) == 1)
        return L1 + (seq_t)n - 1; /* (for n == 0) */

    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t L2 = L0 + L1;
        L0 = L1;
        L1 = L2;
    }
    seq_t luc = L1;                        /* Lucas(n+1) */
    seq_t res = luc + (seq_t)n - 1ull;     /* a(n) = L(n+1) + n - 1 */
    return res;
}

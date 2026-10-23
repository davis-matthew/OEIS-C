/*
 * A017777  Binomial coefficients C(61,n).
 * https://oeis.org/A017777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(61, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 61) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)(61 - (i - 1)) / i;
    }
    return res;
}

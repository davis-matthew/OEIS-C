/*
 * A017776  Binomial coefficients C(60,n).
 * https://oeis.org/A017776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(60,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > (idx_t)60) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
        res = (res * (seq_t)(60 - n + i)) / (seq_t)i;
    return res;
}

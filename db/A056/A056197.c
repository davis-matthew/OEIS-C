/*
 * A056197  Diagonal of A056151.
 * https://oeis.org/A056197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n^2+3n+1) * n! (A056197)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fac = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fac *= (seq_t)i;
    }
    seq_t tn = n;
    seq_t multiplier = (3 * tn * tn) + (3 * tn) + 1;
    return fac * multiplier;
}

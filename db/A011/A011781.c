/*
 * A011781  Sextuple factorial numbers: a(n) = Product_{k=0..n-1} (6*k+3).
 * https://oeis.org/A011781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0}^{n-1} (6*k+3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = (seq_t)(6 * i + 3);
        r *= term;
    }
    return r;
}

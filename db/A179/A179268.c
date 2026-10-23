/*
 * A179268  Product of numbers between and including n and n^2.
 * https://oeis.org/A179268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=n}^{n^2} k
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t end = n * n;                /* n <= 10000, so end fits in idx_t */
    for (idx_t k = n; k <= end; ++k) {
        result *= (seq_t)k;
    }
    return result;
}

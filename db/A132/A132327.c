/*
 * A132327  a(n) = Product{k>=0} (1 + floor(n/3^k)).
 * https://oeis.org/A132327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = PRODUCT{k>=0} (1 + floor(n / 3^k))
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t m = n;
    do {
        prod *= (seq_t)(1 + (seq_t)m);
        m /= 3;
    } while (m > 0);
    return prod;
}

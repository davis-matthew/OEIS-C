/*
 * A132033  Product{0<=k<=floor(log_9(n)), floor(n/9^k)}, n>=1.
 * https://oeis.org/A132033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Π_{k=0}^{⌊log₉(n)⌋} ⌊n / 9ᵏ⌋ (n ≥ 1)
 * Range:     u8 n <= 51, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t d = n; d > 0; d /= 9) {
        prod *= (seq_t)d;
    }
    return prod;
}

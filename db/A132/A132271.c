/*
 * A132271  Product{k>=0, 1+floor(n/10^k)}.
 * https://oeis.org/A132271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k≥0} (1 + ⌊ n / 10^k ⌋)
 * Range:     u8 n <= 49, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t t = n;
    while (t) {
        prod *= (seq_t)(t + 1);
        t /= 10;
    }
    return prod;
}

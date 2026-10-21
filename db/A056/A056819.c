/*
 * A056819  a(n) = Product_{k|n} (n+1-k).
 * https://oeis.org/A056819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k|n} (n+1-k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 29, u64 n <= 71, u128 n <= 239,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 == d2) {
                prod *= ((seq_t)n + 1 - (seq_t)d1);
            } else {
                prod *= ((seq_t)n + 1 - (seq_t)d1);
                prod *= ((seq_t)n + 1 - (seq_t)d2);
            }
        }
    }
    return prod;
}

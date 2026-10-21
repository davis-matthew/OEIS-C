/*
 * A053100  a(n) = ((6*n+7)(!^6))/7, related to A008542 ((6*n+1)(!^6) sextic,
 *          or 6-factorials).
 * https://oeis.org/A053100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} (6*k + 7) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod *= (seq_t)(6 * k + 7);
    }
    return prod;
}

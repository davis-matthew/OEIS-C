/*
 * A135270  Row 6 of A038207.
 * https://oeis.org/A135270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = binom(6,n) * 2^(6-n) for 0 ≤ n ≤ 6, a(n) = 0 for n > 6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 6) return 0;
    static const seq_t binom[7] = {1, 6, 15, 20, 15, 6, 1};
    seq_t res = binom[n];
    seq_t pow2 = ((seq_t)1) << (6 - n);
    return res * pow2;
}

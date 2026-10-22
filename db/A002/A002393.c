/*
 * A002393  Weight distribution of [8,4,4] Hamming code omitting 0 terms.
 * https://oeis.org/A002393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   Weight distribution of [8,4,4] Hamming code - non-zero weight
 *            counts
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)1;
    return (seq_t)0;
}

/*
 * A167858  A000004 preceded by 3, 14, 36, 36, 12.
 * https://oeis.org/A167858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 3, a(1) = 14, a(2) = 36, a(3) = 36, a(4) = 12, a(n) = 0
 *            for n > 4
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use a minimal conditional table for the first five terms,
       returning zero thereafter. */
    if (n == 0u) return (seq_t)3;
    if (n == 1u) return (seq_t)14;
    if (n == 2u) return (seq_t)36;
    if (n == 3u) return (seq_t)36;
    if (n == 4u) return (seq_t)12;
    return (seq_t)0;
}

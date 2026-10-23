/*
 * A159915  a(n) = floor((n+1)/4)*floor(n/2).
 * https://oeis.org/A159915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+1)/4)*floor(n/2)
 * Range:     u8 n <= 46, u16 n <= 725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t floor_n2   = (seq_t)n / (seq_t)2;         /* floor(n/2) */
    seq_t floor_np1_4 = ((seq_t)n + (seq_t)1) / (seq_t)4; /* floor((n+1)/4) */
    return floor_np1_4 * floor_n2;
}

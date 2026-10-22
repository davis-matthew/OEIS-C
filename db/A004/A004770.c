/*
 * A004770  Numbers of the form 8k+5; or, numbers whose binary expansion ends
 *          in 101.
 * https://oeis.org/A004770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n - 3
 * Range:     u8 n <= 32, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * 8 - 3;
}

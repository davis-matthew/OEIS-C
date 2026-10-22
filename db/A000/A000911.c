/*
 * A000911  a(n) = (2n+3)! /( n! * (n+1)! ).
 * https://oeis.org/A000911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+3)! / (n! * (n+1)!)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i <= n + 1; i++) {
        result *= (seq_t)(n + 2 + i);
        if (i < n) {
            result /= (seq_t)(i + 1);
        }
    }
    return result;
}

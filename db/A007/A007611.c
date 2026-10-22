/*
 * A007611  a(n) = n! + 2^n.
 * https://oeis.org/A007611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + 2^n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    seq_t power_of_2 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_of_2 *= 2;
    }
    
    return factorial + power_of_2;
}

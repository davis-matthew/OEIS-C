/*
 * A003523  Divisors of 2^10 - 1.
 * https://oeis.org/A003523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 2^10 - 1 = 1023
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t num = 1023;
    idx_t count = 0;
    
    for (seq_t d = 1; d <= num; d++) {
        if (num % d == 0) {
            count++;
            if (count == n) {
                return d;
            }
        }
    }
    return 0;
}

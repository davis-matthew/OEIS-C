/*
 * A005018  Divisors of 20: a finite sequence associated with the Lie algebra
 *          A_4.
 * https://oeis.org/A005018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 20
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t i = 1; i <= 20; i++) {
        if (20 % i == 0) {
            count++;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
    return 0;
}

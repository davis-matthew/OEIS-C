/*
 * A082802  Smallest multiple of 7 beginning with n.
 * https://oeis.org/A082802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (7 | n) ? n : 10*n + ((7 - (10*n mod 7)) mod 7)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_mod7 = n % 7;
    if (n_mod7 == 0) {
        return n;
    }
    /* n*10 mod 7 */
    seq_t ten_n_mod7 = (n * 10) % 7;
    /* the required remainder to make the number divisible by 7 */
    seq_t r = (7 - ten_n_mod7) % 7;
    /* smallest multiple of 7 that starts with n */
    return n * 10 + r;
}

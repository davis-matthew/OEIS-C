/*
 * A089186  Decreases from 9 * 10^k down to 1, restarting at 9 * 10^(k+1).
 * https://oeis.org/A089186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^(floor(log_10(n)) + 1) - n
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the number of decimal digits of n (n >= 1). */
    idx_t temp = n;
    int digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }
    /* Compute 10^digits in seq_t. */
    seq_t pow10 = 1;
    for (int i = 0; i < digits; ++i) {
        pow10 *= (seq_t)10;
    }
    return pow10 - (seq_t)n;
}

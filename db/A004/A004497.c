/*
 * A004497  Tersum n + 8.
 * https://oeis.org/A004497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n, 8), where tersum adds n and 8 in base 3
 *            digit-wise mod 3
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val_n = (seq_t)n;
    seq_t m = 8;
    seq_t result = 0;
    seq_t power = 1;
    
    while (val_n > 0 || m > 0) {
        seq_t digit_n = val_n % 3;
        seq_t digit_m = m % 3;
        result += ((digit_n + digit_m) % 3) * power;
        power *= 3;
        val_n /= 3;
        m /= 3;
    }
    
    return result;
}

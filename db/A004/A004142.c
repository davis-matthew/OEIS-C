/*
 * A004142  a(n) = n*(3^n - 2^n).
 * https://oeis.org/A004142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^n - 2^n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t power_3 = power((seq_t)3, n);
    seq_t power_2 = power((seq_t)2, n);
    return (seq_t)n * (power_3 - power_2);
}

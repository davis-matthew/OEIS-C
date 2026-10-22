/*
 * A007396  Add 2, then reverse digits!.
 * https://oeis.org/A007396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(a(n-1) + 2), a(0) = 0
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t num) {
    seq_t result = 0;
    while (num > 0) {
        result = result * 10 + (num % 10);
        num /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n >= 6) {
        n = 6 + ((n - 6) % 81);
    }
    
    seq_t current = 0;
    for (idx_t i = 1; i <= n; i++) {
        current = reverse_digits(current + 2);
    }
    return current;
}

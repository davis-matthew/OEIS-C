/*
 * A007399  Add 8, then reverse digits!.
 * https://oeis.org/A007399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = reverse_digits(a(n) + 8), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t n) {
    seq_t result = 0;
    while (n > 0) {
        result = result * 10 + (n % 10);
        n /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t value = 0;
    for (idx_t i = 0; i < n; i++) {
        value = reverse_digits(value + 8);
    }
    return value;
}

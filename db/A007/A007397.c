/*
 * A007397  Add 5, then reverse digits!.
 * https://oeis.org/A007397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_digits(a(n-1) + 5), a(0) = 0
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t num) {
    seq_t result = 0;
    while (num > 0) {
        result = result * 10 + num % 10;
        num /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t current = 0;
    for (idx_t i = 0; i < n; i++) {
        current = reverse_digits(current + 5);
    }
    return current;
}

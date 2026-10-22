/*
 * A002108  4th powers written backwards.
 * https://oeis.org/A002108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000583(n)) where A000583(n) = n^4 and A004086
 *            reverses digits
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 177, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t x) {
    seq_t result = 0;
    while (x > 0) {
        result = result * 10 + (x % 10);
        x /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n2 = n_val * n_val;
    seq_t n4 = n2 * n2;
    return reverse_digits(n4);
}

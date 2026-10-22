/*
 * A006960  Reverse and Add! sequence starting with 196.
 * https://oeis.org/A006960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = A056964^n(196), where A056964(x) = x + reverse_digits(x)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 17, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_number(seq_t num) {
    seq_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    return reversed;
}

seq_t a(idx_t n) {
    seq_t current = 196;
    for (idx_t i = 0; i < n; i++) {
        seq_t reversed = reverse_number(current);
        current = current + reversed;
    }
    return current;
}

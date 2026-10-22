/*
 * A004165  Cubes written backwards.
 * https://oeis.org/A004165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000578(n)) = reverse_digits(n^3)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 1001, u64 n <= 10000,
 *            u128 n <= 10000, big any n
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
    seq_t n_seq = (seq_t)n;
    seq_t cube = n_seq * n_seq * n_seq;
    return reverse_digits(cube);
}

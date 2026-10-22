/*
 * A001734  6 in base 10-n.
 * https://oeis.org/A001734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal digits of 6 written in base 10-n, for 0 <= n <= 9
 *            (base 1 is unary: 6 written as six 1's). Finite sequence
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 9) {
        return 0;
    }

    unsigned base = (unsigned)(10 - n);
    unsigned v = 6;
    unsigned digits[16];
    int nd = 0;

    if (base == 1) {
        /* unary representation: v copies of the digit 1 */
        unsigned i;
        for (i = 0; i < v; i++) {
            digits[nd++] = 1;
        }
    } else {
        while (v > 0) {
            digits[nd++] = v % base;
            v /= base;
        }
    }

    seq_t result = 0;
    int i;
    for (i = nd - 1; i >= 0; i--) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}

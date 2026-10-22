/*
 * A005058  a(n) = 5^n - 3^n.
 * https://oeis.org/A005058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 3^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    return power((seq_t)5, n) - power((seq_t)3, n);
}

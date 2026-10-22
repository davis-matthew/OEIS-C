/*
 * A001117  a(n) = 3^n - 3*2^n + 3.
 * https://oeis.org/A001117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*2^n + 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
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
    seq_t pow3 = power((seq_t)3, n);
    seq_t pow2 = power((seq_t)2, n);
    return pow3 - 3 * pow2 + 3;
}

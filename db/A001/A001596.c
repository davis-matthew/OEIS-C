/*
 * A001596  a(n) = 7^n + n^7.
 * https://oeis.org/A001596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    return power((seq_t)7, n) + power((seq_t)n, 7);
}

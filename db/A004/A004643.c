/*
 * A004643  Powers of 2 written in base 4.
 * https://oeis.org/A004643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (3-(-1)^n)/2 * 10^floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 2;
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result *= 10;
    }
    if (n % 2 == 1) {
        result *= 2;
    }
    return result;
}

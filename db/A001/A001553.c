/*
 * A001553  a(n) = 1^n + 2^n + ... + 6^n.
 * https://oeis.org/A001553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..6} k^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    int k;
    for (k = 1; k <= 6; k++) {
        seq_t p = 1;
        idx_t i;
        for (i = 0; i < n; i++) {
            p *= k;
        }
        result += p;
    }
    return result;
}

/*
 * A001147  Double factorial of odd numbers: a(n) = (2*n-1)!! =
 *          1*3*5*...*(2*n-1).
 * https://oeis.org/A001147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core, changed
 * Formula:   a(n) = (2*n-1)!! = 1*3*5*...*(2*n-1), with a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result *= (2 * i - 1);
    }
    return result;
}

/*
 * A000457  Exponential generating function: (1+3*x)/(1-2*x)^(7/2).
 * https://oeis.org/A000457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n+3)!!/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)(n + 1);
    for (idx_t i = 5; i <= 2 * n + 3; i += 2) {
        result *= (seq_t)i;
    }
    return result;
}

/*
 * A035276  One seventh of deca-factorial numbers.
 * https://oeis.org/A035276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   7*a(n) = Product_{j=1..n} (10*j-3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t result = 1;
    for (idx_t j = 1; j <= n; ++j) {
        result *= (seq_t)(10 * j - 3);
    }
    result /= 7;
    return result;
}

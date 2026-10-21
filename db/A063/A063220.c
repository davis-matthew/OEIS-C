/*
 * A063220  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          55 ).
 * https://oeis.org/A063220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, a(2) = 10, a(3) = 18, for n >= 4: a(n) = 22 +
 *            20*floor((n-4)/3) + 8*[(n-4)%3 >= 1] + 8*[(n-4)%3 >= 2]
 * Range:     u8 n <= 38, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 3;
    if (n == 2) return 10;
    if (n == 3) return 18;
    /* n >= 4 */
    seq_t k = (seq_t)(n - 4);   /* k >= 0 */
    seq_t q = k / 3;
    seq_t r = k % 3;
    seq_t result = 22 + 20 * q;
    if (r >= 1) result += 8;
    if (r >= 2) result += 8;
    return result;
}

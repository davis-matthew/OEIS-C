/*
 * A001193  a(n) = (n+1)*(2*n)!/(2^n*n!) = (n+1)*(2n-1)!!.
 * https://oeis.org/A001193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * (2n-1)!! = (n+1) * product(k=1 to n, 2*k - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n + 1;
    for (idx_t k = 1; k <= n; k++) {
        result *= (seq_t)(2 * k - 1);
    }
    return result;
}

/*
 * A099933  Main diagonal of array A007754.
 * https://oeis.org/A099933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) where T(i,k) = (i+k)*T(i-1,k) - T(i-2,k), T(0,k) =
 *            1, T(-1,k) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev2 = 0;
    seq_t prev1 = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t cur = (seq_t)(i + n) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

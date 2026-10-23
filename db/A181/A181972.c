/*
 * A181972  Number of integer pairs (x,y) such that 0<x<y<=n and
 *          x*y<=floor(n/2).
 * https://oeis.org/A181972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y): 0 < x < y <= n and x*y <= floor(n/2)}. Computed
 *            by summing, for each y, the number of x satisfying 1 <= x < y
 *            and x <= floor(floor(n/2)/y)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t count = 0;
    seq_t m = (seq_t)n / 2;           /* floor(n/2)   */
    for (idx_t y = 2; y <= n; ++y) {
        seq_t maxx = m / y;          /* floor(m / y) */
        if (maxx >= y) {
            count += (seq_t)(y - 1); /* x < y, so y-1 possibilities   */
        } else {
            count += maxx;           /* limit by product constraint    */
        }
    }
    return count;
}

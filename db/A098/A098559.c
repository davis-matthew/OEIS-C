/*
 * A098559  Expansion of e.g.f. (1+3*x)/(1-3*x).
 * https://oeis.org/A098559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n*n! for n >= 1, a(0) = 1 (taken from the OEIS entry)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)2;          /* start with 2 */
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)3;           /* multiply by 3 once per iteration  → 3^n */
        res *= (seq_t)i;           /* multiply by the factorial factor  → n!    */
    }
    return res;                    /* res = 2 * 3^n * n! */
}

/*
 * A063118  Dimension of the space of weight 2n cusp forms for Gamma_0(50).
 * https://oeis.org/A063118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 for n = 1 a(n) = (30*n + (-1)^n - 27)/2 for n > 1
 * Range:     u8 n <= 9, u16 n <= 2376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 2;
    seq_t t = (seq_t)30 * (seq_t)n;
    /* (-1)^n: +1 if even, -1 if odd */
    if (n & 1) t -= 1;   /* odd n: subtract 1 */
    else      t += 1;   /* even n: add 1    */
    t -= 27;
    return t / 2;
}

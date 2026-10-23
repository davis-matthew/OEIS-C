/*
 * A019462  Add 1, multiply by 1, add 2, multiply by 2, etc., start with 3.
 * https://oeis.org/A019462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3 a(n) = a(n-1) + (n+1)/2 if n is odd a(n) = a(n-1) *
 *            (n/2) if n is even
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 39, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;

    seq_t res = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & (idx_t)1) {           /* odd index: add (i+1)/2 */
            seq_t add = ((seq_t)i + 1) / 2;
            res = res + add;
        } else {                       /* even index: multiply by i/2 */
            seq_t mul = (seq_t)(i / 2);
            res = res * mul;
        }
    }
    return res;
}

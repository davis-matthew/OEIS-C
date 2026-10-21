/*
 * A052626  (2^n+2)*n!.
 * https://oeis.org/A052626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + 2) * n! (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1;   /* 2^0 */
    seq_t fact = 1;   /* 0! = 1 */

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;               /* 2^(i+1) */
        fact *= (seq_t)(i + 1);   /* (i+1)!   */
    }

    return fact * (pow2 + 2);
}

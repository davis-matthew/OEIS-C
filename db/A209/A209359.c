/*
 * A209359  a(n) = 2^n * (n^4 - 4*n^3 + 18*n^2 - 52*n + 75) - 75.
 * https://oeis.org/A209359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n^4 - 4*n^3 + 18*n^2 - 52*n + 75) - 75
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 42, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = n;
    seq_t n2 = n_t * n_t;          /* n^2 */
    seq_t n3 = n2 * n_t;           /* n^3 */
    seq_t n4 = n3 * n_t;           /* n^4 */

    seq_t poly = n4
                 - (seq_t)4 * n3
                 + (seq_t)18 * n2
                 - (seq_t)52 * n_t
                 + (seq_t)75;

    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;           /* double each step */
    }

    seq_t result = pow2 * poly - (seq_t)75;
    return result;
}

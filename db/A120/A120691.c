/*
 * A120691  First differences of coefficients in the continued fraction for e.
 * https://oeis.org/A120691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(3*n-1)=2*n-1, a(3*n)=1-2*n, a(3*n+1)=0, a(0)=2, a(1)=-1
 * Range:     i8 n <= 179, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special cases for n = 0 and n = 1 */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)(-1);

    /* Work in seq_t domain to avoid overflow in intermediate computations */
    seq_t ns = (seq_t)n;
    switch (n % (idx_t)3) {
        case 0: /* n ≡ 0 (mod 3), a(n) = (3 - 2*n)/3 */
            return ((seq_t)3 - (seq_t)2 * ns) / (seq_t)3;
        case 1: /* n ≡ 1 (mod 3), a(n) = 0 */
            return (seq_t)0;
        case 2: /* n ≡ 2 (mod 3), a(n) = (2*n - 1)/3 */
            return ((seq_t)2 * ns - (seq_t)1) / (seq_t)3;
    }
    /* unreachable but keep compiler happy */
    return (seq_t)0;
}

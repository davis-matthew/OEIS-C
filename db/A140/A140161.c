/*
 * A140161  a(1)=1, a(n) = a(n-1) + n^4 if n odd, a(n) = a(n-1) + n^5 if n is
 *          even.
 * https://oeis.org/A140161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = a(n-1) + (n^4 if n odd, n^5 if n even)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t k = (seq_t)i;
        if (i & 1) {
            /* n is odd: add n^4 */
            seq_t k2 = k * k;
            result += k2 * k2;
        } else {
            /* n is even: add n^5 */
            seq_t k2 = k * k;
            seq_t k4 = k2 * k2;
            result += k4 * k;
        }
    }
    return result;
}

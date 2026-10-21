/*
 * A051189  Octo-factorial numbers.
 * https://oeis.org/A051189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*8^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Octo-factorial: a(0)=1, and a(n) = product_{k=1..n} 8*k for n>=1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        result *= (seq_t)i;   /* multiply by k */
        result *= (seq_t)8;   /* multiply by 8 */
    }
    return result;
}

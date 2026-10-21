/*
 * A067056  a(n) = (1)*(2 + 3 + 4 + ... + n) + (1 + 2)*(3 + 4 + 5 + ... + n) +
 *          (1 + 2 + 3)*(4 + 5 + 6 + ... + n) + ... + (1 + 2 + 3 + ... +
 *          n-1)*n.
 * https://oeis.org/A067056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^4 + 5*n^3 - 5*n - 2)/60, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t t  = (seq_t)n;           /* n */
    seq_t n2 = t * t;              /* n^2 */
    seq_t n3 = n2 * t;             /* n^3 */
    seq_t n4 = n2 * n2;            /* n^4 */

    /* 2*n^4 + 5*n^3 - 5*n - 2 */
    seq_t num = 2 * n4 + 5 * n3 - 5 * t - 2;

    /* Final result: n * num / 60 */
    seq_t result = t * num / 60;
    return result;
}

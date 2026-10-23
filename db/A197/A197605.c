/*
 * A197605  Floor( ( n + 1/n )^6 ).
 * https://oeis.org/A197605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/n)^6). For n > 3, a(n) = n^6 + 6*n^4 +
 *            15*n^2 + 20
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 64) */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)64;
    if (n == 2)
        return (seq_t)244;
    if (n == 3)
        return (seq_t)1371;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n6 = n4 * n2;        /* n^6 */

    return n6 + (seq_t)6 * n4 + (seq_t)15 * n2 + (seq_t)20;
}

/*
 * A061316  a(n) = n*(n+1)*(n^2 + n + 4)/4.
 * https://oeis.org/A061316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 + n + 4)/4
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* Cast n to seq_t before doing any arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute n*(n+1) and n^2 + n + 4, both divisible by 2 */
    seq_t prod1 = nn * (nn + 1);          /* n*(n+1)   */
    seq_t prod2 = nn * nn + nn + (seq_t)4; /* n^2 + n + 4 */

    /* Perform the forced division by 2 on each product */
    seq_t half1 = prod1 / (seq_t)2;
    seq_t half2 = prod2 / (seq_t)2;

    /* Final product gives a(n) */
    return half1 * half2;
}

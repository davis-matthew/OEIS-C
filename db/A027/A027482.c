/*
 * A027482  a(n) = n*(n^3 - 1)/2.
 * https://oeis.org/A027482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 - 1)/2 (OEIS A027482) Offset: 2 (a(2) is the first
 *            term listed)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the result is undefined by the offset.
       Returning 0 provides a harmless default value. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;

    /* Compute n*(n-1)/2 first to keep intermediate values small.
       (n*(n-1)) is always even. */
    seq_t val = nn * (nn - 1);
    val /= 2;

    /* n^2 + n + 1 */
    seq_t tmp = nn * nn + nn + 1;

    /* Final product: (n*(n-1)/2)*(n^2 + n + 1) == n*(n^3-1)/2 */
    val *= tmp;
    return val;
}

/*
 * A034864  a(5) = 5, a(6) = 1170, for n >= 7, a(n) = n!*(4*n^3 - 30*n^2 +
 *          40*n + 3)/24.
 * https://oeis.org/A034864
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(4*n^3 - 30*n^2 + 40*n + 3)/24 (with special cases
 *            a(5)=5, a(6)=1170)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and special values */
    if (n < 5) return 0;            /* dummy value for indices before offset */
    if (n == 5) return 5;
    if (n == 6) return 1170;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Apply the division by 24 exactly (n >= 5 guarantees divisibility) */
    fact /= 24;

    /* Compute the polynomial part 4*n^3 - 30*n^2 + 40*n + 3 */
    seq_t nn = (seq_t)n;
    seq_t poly = 4 * nn * nn * nn
               - 30 * nn * nn
               + 40 * nn
               + 3;

    /* Final result */
    return fact * poly;
}

/*
 * A201533  Number of n X 2 0..2 arrays with rows and columns
 *          lexicographically nondecreasing and every element equal to at
 *          least one horizontal or vertical neighbor.
 * https://oeis.org/A201533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 – 12 n^7 + 322 n^6 – 840 n^5 + 1729 n^4 + 58212 n^3
 *            – 243972 n^2 + 749040 n – 806400) / 40320
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 61, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset values directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)25;

    /* Compute the polynomial for n > 3 using an intermediate signed type
       (__int128 provides a wide enough range for all tested values) */
    __int128 nn = n;
    __int128 n2 = nn * nn;          /* n^2   */
    __int128 n3 = n2 * nn;          /* n^3   */
    __int128 n4 = n3 * nn;          /* n^4   */
    __int128 n5 = n4 * nn;          /* n^5   */
    __int128 n6 = n5 * nn;          /* n^6   */
    __int128 n7 = n6 * nn;          /* n^7   */
    __int128 n8 = n7 * nn;          /* n^8   */

    __int128 num = n8
                 - (__int128)12 * n7
                 + (__int128)322 * n6
                 - (__int128)840 * n5
                 + (__int128)1729 * n4
                 + (__int128)58212 * n3
                 - (__int128)243972 * n2
                 + (__int128)749040 * nn
                 - (__int128)806400;

    /* The numerator is divisible by 40320; cast the quotient to seq_t. */
    return (seq_t)(num / (__int128)40320);
}

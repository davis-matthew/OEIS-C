/*
 * A160840  Expansion of
 *          (1+147*x+1230*x^2+1885*x^3+714*x^4+63*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (449*n^6 + 1803*n^5 + 3565*n^4 + 3725*n^3 + 2106*n^2 +
 *            592*n + 80) / 80
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1218,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use an intermediate type large enough for all intermediate products.
       __int128 is available on GCC/Clang and is sufficient for n up to at least 10⁴. */
    __int128 nn = (__int128)n;

    /* Compute powers of n */
    __int128 n2 = nn * nn;          /* n^2   */
    __int128 n3 = n2 * nn;          /* n^3   */
    __int128 n4 = n3 * nn;          /* n^4   */
    __int128 n5 = n4 * nn;          /* n^5   */
    __int128 n6 = n5 * nn;          /* n^6   */

    /* Assemble the numerator of the rational expression. */
    __int128 num = 449 * n6
                  + 1803 * n5
                  + 3565 * n4
                  + 3725 * n3
                  + 2106 * n2
                  + 592  * nn
                  + 80;

    /* Final division gives the exact integer value. */
    __int128 res = num / 80;

    /* Cast to the user‑supplied seq_t type.  The cast is exact whenever the
       result fits in seq_t; otherwise the value simply wraps as per the
       integer type. */
    return (seq_t)res;
}

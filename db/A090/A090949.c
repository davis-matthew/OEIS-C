/*
 * A090949  a(n) = (1/24)*(n+1)*(3*n^3+59*n^2+358*n+648).
 * https://oeis.org/A090949
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(n+1)*(3*n^3+59*n^2+358*n+648)
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 425, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A090949 has offset -1; values for n < -1 are undefined, return 0. */
    if (n < -1)
        return 0;

    /* Use signed __int128 for intermediate arithmetic. */
    __int128 nn = n;
    __int128 term = (nn + 1) *
        ((__int128)3 * nn * nn * nn +
         (__int128)59 * nn * nn +
         (__int128)358 * nn +
         (__int128)648);
    term /= 24;
    return (seq_t)term;
}

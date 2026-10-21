/*
 * A061004  Nearest integer to n^6/36.
 * https://oeis.org/A061004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^6/36 Equivalent to: a(n) =
 *            floor((n^6 + 18) / 36) for n >= 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 N   = (unsigned __int128)n;   /* n */
    unsigned __int128 N2  = N * N;                  /* n^2 */
    unsigned __int128 N4  = N2 * N2;                /* n^4 */
    unsigned __int128 N6  = N4 * N2;                /* n^6 */
    unsigned __int128 sum = N6 + 18;                /* n^6 + 18 */
    unsigned __int128 res = sum / 36;               /* floor((n^6+18)/36) */
    return (seq_t)res;
}

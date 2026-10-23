/*
 * A171129  a(n)=(n^4-n^3-n^2-n)/2.
 * https://oeis.org/A171129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n^4 - n^3 - n^2 - n)/2
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type for intermediate calculations to avoid overflow
       before truncating to seq_t. __int128 is available in C++17 and commonly
       supported by modern C compilers. */
    __int128 nn  = ( __int128 ) n;
    __int128 nn2 = nn * nn;
    __int128 nn3 = nn2 * nn;
    __int128 nn4 = nn3 * nn;

    __int128 res = (nn4 - nn3 - nn2 - nn) / 2;
    return (seq_t)res;
}

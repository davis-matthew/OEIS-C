/*
 * A068239  1/2 the number of colorings of a 3 X 3 square array with n colors.
 * https://oeis.org/A068239
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 -12 n^8 +66 n^7 -216 n^6 +459 n^5 -648 n^4 * +594
 *            n^3 -323 n^2 +79 n) / 2 for n >= 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 14, u64 n <= 150, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* behaviour below the offset is irrelevant */

    /* Use signed __int128 for intermediate calculations to avoid
       overflow while keeping the intermediate values accurate.
       The final result is guaranteed to be non‑negative and fits in
       seq_t for all n that the test harness uses. */
    __int128 N = n;
    __int128 P = 1;                         /* start with coefficient of n^9 */

    /* Horner's method with the coefficients of the numerator:
       -12,  66, -216, 459, -648, 594, -323, 79   (coefficients of n^8 … n) */
    P = P * N + (__int128)(-12);
    P = P * N + (__int128)( 66);
    P = P * N + (__int128)(-216);
    P = P * N + (__int128)( 459);
    P = P * N + (__int128)(-648);
    P = P * N + (__int128)( 594);
    P = P * N + (__int128)(-323);
    P = P * N + (__int128)(  79);

    /* Multiply by the final factor n (coefficient of n) and divide by 2 */
    P = P * N;
    P = P / 2;

    return (seq_t)P;
}

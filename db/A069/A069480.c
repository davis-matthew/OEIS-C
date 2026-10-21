/*
 * A069480  Determinant of rank n matrix of 1..n^2 filled successively along
 *          antidiagonals.
 * https://oeis.org/A069480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n+2)/4 (n ≡ 0 mod 4) Formula: a(n) = (n^2+1)/2 (n ≡ 1
 *            mod 4) Formula: a(n) = -n*(n+2)/4 (n ≡ 2 mod 4) Formula: a(n) =
 *            -(n^2+1)/2 (n ≡ 3 mod 4)
 * Range:     i8 n <= 16, i16 n <= 256, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result;

    switch (n & 3ULL)  /* n % 4 */
    {
        case 0:
            result = N * (N + 2) / 4;
            break;
        case 1:
            result = (N * N + 1) / 2;
            break;
        case 2:
            result = -(N * (N + 2) / 4);
            break;
        case 3:
            result = -((N * N + 1) / 2);
            break;
    }

    return result;
}

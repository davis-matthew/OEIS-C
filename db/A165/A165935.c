/*
 * A165935  a(n) = (-1)^(n-1)*n*(4n^2-5)^2.
 * https://oeis.org/A165935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1) * n * (4n^2-5)^2
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 42, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – not used, but safe */
    __int128 nn = n;                /* use 128‑bit intermediary to avoid UB */
    __int128 t  = 4 * nn * nn - 5;  /* 4n² - 5   */
    __int128 t2 = t * t;            /* (4n² - 5)² */
    __int128 res = nn * t2;         /* n * (4n² - 5)² */

    if ((n & 1ull) == 0)            /* (-1)^(n-1) : negative for even n */
        res = -res;

    return (seq_t)res;              /* cast to the build’s seq_t type */
}

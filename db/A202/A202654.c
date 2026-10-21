/*
 * A202654  Number of ways to place 3 nonattacking semi-queens on an n X n
 *          board.
 * https://oeis.org/A202654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/6*(n-2)*(n-1)*n*(n^3-5*n^2+8*n-3)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 55, u64 n <= 2192,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the value is 0 by the formula. */
    if (n < 3)
        return 0;

    unsigned __int128 tn  = (unsigned __int128)n;
    /* Compute (n-2)*(n-1)*n / 6 using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 prod = (tn - 2) * (tn - 1) * tn;
    prod /= 6;

    /* Compute n^3-5*n^2+8*n-3 in 128‑bit. */
    unsigned __int128 poly =
        tn * tn * tn - (unsigned __int128)5 * tn * tn
        + (unsigned __int128)8 * tn - 3;

    unsigned __int128 result = prod * poly;
    return (seq_t)result;
}

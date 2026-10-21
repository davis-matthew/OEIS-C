/*
 * A222309  Let P be a one-move "rider" with move set M={(1,2)}; a(n) is the
 *          number of non-attacking positions of three indistinguishable
 *          pieces P on an n X n board.
 * https://oeis.org/A222309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6/6 - 5*n^5/24 + n^4/16 - 11*n^3/48 + 7*n^2/48 + 1/32 +
 *            (-1)^n*(n^3/16 - n^2/16 - 1/32)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For n = 0 the result is 0, but the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* Use a wider type for intermediate calculations to avoid overflow. */
    unsigned __int128 N = (unsigned long long)n;
    unsigned __int128 n2  = N * N;          /* n^2   */
    unsigned __int128 n3  = n2 * N;         /* n^3   */
    unsigned __int128 n4  = n3 * N;         /* n^4   */
    unsigned __int128 n5  = n4 * N;         /* n^5   */
    unsigned __int128 n6  = n5 * N;         /* n^6   */

    /* Numerator of the rational expression, using common denominator 96. */
    unsigned __int128 base = 16 * n6 - 20 * n5 + 6 * n4 - 22 * n3 + 14 * n2 + 3;
    unsigned __int128 parity = 6 * n3 - 6 * n2 - 3;
    unsigned __int128 term = (n & 1) ? (base - parity) : (base + parity);

    /* Division by 96 is exact for all integer n. */
    unsigned __int128 result = term / 96;

    return (seq_t)result;
}

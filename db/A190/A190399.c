/*
 * A190399  Number of ways to place 4 nonattacking grasshoppers on a toroidal
 *          chessboard of size n x n.
 * https://oeis.org/A190399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/24 * n^2 * (n^6 - 6*n^4 - 96*n^3 + 347*n^2 + 96*n - 726
 *            + 96*(-1)^n) (n > 4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases n = 1..4 are defined by the OEIS listing. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)54;
    if (n == 4) return (seq_t)1068;

    /* For n > 4 use the closed‑form formula.
       We perform all intermediate arithmetic in signed 128‑bit
       to avoid overflow for the required n range.          */
    unsigned __int128 ns   = (unsigned __int128)n;      /* n */
    unsigned __int128 n2   = ns * ns;                   /* n^2 */
    unsigned __int128 n3   = n2 * ns;                   /* n^3 */
    unsigned __int128 n4   = n3 * ns;                   /* n^4 */
    unsigned __int128 n6   = n4 * n2;                   /* n^6 */

    /* Build the polynomial part.  Use signed __int128 to handle
       possible intermediate negatives (the final result is always positive). */
    __int128 expr = (__int128)n6;
    expr -= (__int128)6 * n4;
    expr -= (__int128)96 * n3;
    expr += (__int128)347 * n2;
    expr += (__int128)96 * ns;
    expr -= (__int128)726;
    /* 96*(-1)^n contributes +96 for even n and -96 for odd n. */
    if ( (n & 1) == 0 )
        expr += (__int128)96;
    else
        expr -= (__int128)96;

    __int128 prod = (__int128)n2 * expr;   /* n^2 * expr */
    /* Division by 24 is exact by definition of the formula. */
    __int128 result = prod / 24;
    return (seq_t)result;
}

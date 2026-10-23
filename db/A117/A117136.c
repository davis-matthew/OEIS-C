/*
 * A117136  Triangle read by rows: row n always begins with 1. Later terms in
 *          row n are obtained by successively subtracting or adding the
 *          numbers n, n+1, n+2, until reaching 1 again. The rule is that you
 *          subtract if the result is positive, otherwise you add.
 * https://oeis.org/A117136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A117136:  Triangle read by rows.
   Triangle definition:
     T(n,0) = 1
     for 0<k<=n+1:
        T(n,2k-1) = n+2-k
        T(n,2k)   = 2n+1+k
   The sequence a(n) is obtained by reading the triangle row‑wise, starting
   with row 0.  Row n has length 2n+2.
   The code maps an index n to its row and column, then evaluates the
   above formula using only integer arithmetic, compatible with C11 and C++17.
*/

#include <stdint.h>

/* Integer square root of a 128‑bit number, returning the floor of sqrt(x). */
static idx_t
isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0, high = ~(idx_t)0, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t
a(idx_t n)
{
    /* Find the row r such that r(r+1) <= n < (r+1)(r+2). */
    unsigned __int128 four_n  = (unsigned __int128)4 * n + 1; /* 1 + 4n */
    idx_t          r = (isqrt_u128(four_n) - 1) >> 1;          /* floor((sqrt(1+4n)-1)/2) */

    /* First index of this row. */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1);
    idx_t start = (idx_t)start128;

    /* Position inside the row. */
    idx_t c = n - start;  /* 0 <= c <= 2r+1 */

    if (c == 0) {
        return (seq_t)1;
    }
    if (c & 1) {   /* odd index: 2k-1 */
        idx_t k = (c + 1) >> 1;          /* k = (c+1)/2,   1 <= k <= r+1 */
        seq_t val = (seq_t)r + (seq_t)2 - (seq_t)k;
        return val;
    } else {       /* even index: 2k */
        idx_t k = c >> 1;                 /* k = c/2,       1 <= k <= r+1 */
        seq_t val = (seq_t)2 * (seq_t)r + (seq_t)1 + (seq_t)k;
        return val;
    }
}

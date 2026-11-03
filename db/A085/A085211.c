/*
 * A085211  Array A(x,y): concatenation of the binary expansions of x & y in
 *          such a way that 'y' is inserted after the least significant 1-bit
 *          of 'x', followed by the remaining 0-bits, if any. Listed
 *          antidiagonalwise as A(0,0), A(1,0), A(0,1), A(2,0), A(1,1),
 *          A(0,2), ... Zero is expanded as an empty string.
 * https://oeis.org/A085211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A085211.  
   a(n) lists the values A(x,y) antidiagonally: 
       (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),(3,0),... 
   where A(x,y) concatenates the binary expansion of x with that of y
   by inserting y immediately after the least‑significant 1‑bit of x
   (the trailing zeros of x).  Zero expands to an empty string, so
   A(0,y)=y and A(x,0)=x.  
   The function is O(√n) for locating the pair (x,y) and O(1) thereafter.
*/

#include <stdint.h>

static seq_t A_xy(idx_t x, idx_t y)
{
    /* Handles the case x = 0 separately */
    if (x == 0)
        return y;

    /* trailing zeros of x: t = v_2(x) */
    idx_t t = 0;
    idx_t tmp = x;
    while ((tmp & 1) == 0) {
        ++t;
        tmp >>= 1;
    }

    /* higher part of x above the least‑significant 1‑bit */
    seq_t x_high = x >> (t + 1);   /* safe shift: t+1 <= 64 for u64 builds */

    /* length of binary representation of y (0 → length 0) */
    idx_t len_y = 0;
    if (y != 0) {
        seq_t ytmp = y;
        while (ytmp) {
            ++len_y;
            ytmp >>= 1;
        }
    }

    /* construct the result using 128‑bit arithmetic to avoid UB for large shifts */
    unsigned __int128 res = 0;
    res |= (unsigned __int128)x_high << (1 + len_y + t);
    res |= (unsigned __int128)1 << (len_y + t);
    res |= (unsigned __int128)y << t;

    return (seq_t)res;
}

/* Find the unique pair (x,y) corresponding to index n (n >= 0) via antidiagonal walk */
static void index_to_pair(idx_t n, idx_t *x, idx_t *y)
{
    /* Find the row s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s * (s + 1)) / 2 <= n) {
        ++s;
    }
    --s;                            /* now s satisfies the inequality */

    idx_t start = (s * (s + 1)) / 2; /* first index of row s */
    idx_t j = n - start;            /* position within the row (0 <= j <= s) */
    *x = s - j;                     /* x decreases from s to 0 */
    *y = j;
}

extern seq_t a(idx_t n)
{
    idx_t x, y;
    index_to_pair(n, &x, &y);
    return A_xy(x, y);
}

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
 * Helpers:   A_xy, index_to_pair
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
#include "../utils/A_xy.h"
#include "../utils/index_to_pair.h"

extern seq_t a(idx_t n)
{
    idx_t x, y;
    index_to_pair(n, &x, &y);
    return A_xy(x, y);
}

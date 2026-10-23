/*
 * A032889  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(0) < d(1) > d(2) < ...
 * https://oeis.org/A032889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Computes the nth term of OEIS A032889.
   A032889(n) = the nth positive integer whose base-10 digits
   satisfy the alternation d0 < d1 > d2 < d3 > d4 ...
   (digits are taken from least significant to most).  
   This function scans integers sequentially until it finds the
   specified term.  It works for any order of calls and any index
   n ≥ 1.  
*/

#include <stdbool.h>
#include "../utils/zigzag.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;  /* number of valid terms found so far */
    seq_t num = 0;    /* current candidate number */

    /* Scan until the nth term is found */
    while (count < n) {
        ++num;              /* next candidate */
        if (zigzag(num))
            ++count;
    }
    return num;
}

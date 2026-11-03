/*
 * A104763  Triangle read by rows: Fibonacci(1), Fibonacci(2), ...,
 *          Fibonacci(n) in row n.
 * https://oeis.org/A104763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 107 terms through n = 4710 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS sequence A104763: 
   The reluctant sequence of Fibonacci numbers except 0.
   For a given n (n >= 1), find the row r such that
      r*(r+1)/2 >= n  and  (r-1)*r/2 < n.
   Let col = n - (r-1)*r/2   (1‑based column in that row).  
   Return the Fibonacci number F(col), where F(1)=F(2)=1.
*/

#include <stdint.h>

/* Compute F(k) for k >= 1 with F(1)=1, F(2)=1 */
static seq_t fibonacci(idx_t k)
{
    if (k == 1 || k == 2)
        return (seq_t)1;
    seq_t a = (seq_t)1;   /* F(1) */
    seq_t b = (seq_t)1;   /* F(2) */
    for (idx_t i = 3; i <= k; ++i) {
        seq_t tmp = b;
        b = a + b;
        a = tmp;
    }
    return b;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but harmless */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (1) {
        unsigned __int128 cum = (unsigned __int128)r * (r + 1) / 2;
        if (cum >= (unsigned __int128)n)
            break;
        ++r;
    }

    /* Previous cumulative count before this row */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t col = n - (idx_t)prev;   /* 1‑based column within the row */

    return fibonacci(col);
}

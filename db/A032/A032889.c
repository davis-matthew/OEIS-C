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
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
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

/* Test whether a number satisfies the alternating pattern. */
static bool zigzag(seq_t x)
{
    /* Single‑digit numbers trivially satisfy the pattern. */
    if (x < 10) return true;

    seq_t prev = x % 10;   /* d0 */
    x /= 10;
    bool expect_inc = true;  /* d1 > d0 */

    while (x > 0) {
        seq_t d = x % 10;   /* next digit */
        if (expect_inc) {
            if (d <= prev) return false;   /* should be greater */
        } else {
            if (d >= prev) return false;   /* should be smaller */
        }
        prev = d;
        expect_inc = !expect_inc;
        x /= 10;
    }
    return true;
}

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

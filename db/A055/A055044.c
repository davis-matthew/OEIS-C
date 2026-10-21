/*
 * A055044  Numbers of the form 2^(2i+1)*(8*j+1).
 * https://oeis.org/A055044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   term_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of numbers of the form 2^(2i+1)*(8*j+1) (i,j ≥ 0),
   ordered increasingly.  This implementation scans natural numbers
   and returns the nth term. */
#include <stdbool.h>
#include "../utils/term_ok.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* defined only for n ≥ 1 */
    seq_t cur = 2;           /* first candidate */
    idx_t count = 0;

    while (true) {
        if (term_ok(cur)) {
            count++;
            if (count == n) return cur;
        }
        cur += 1;
    }
}

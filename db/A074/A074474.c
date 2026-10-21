/*
 * A074474  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 4n+3.
 * https://oeis.org/A074474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A074474
 *  a(n) = the index (starting at 1) of the first term in the Collatz
 *  trajectory of the integer 4n-1 that is strictly smaller than 4n-1.
 *  The sequence is defined for n ≥ 1; values are computed on demand.
 *
 *  Example: n = 6 → start = 23
 *  Trajectory: 23,70,35,106,53,160,80,40,20,…
 *  The first term below 23 is 20 (9th term), hence a(6) = 9.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Starting value: 4n-1  (which is ≡ 3 (mod 4)) */
    seq_t start = (seq_t)4 * n - 1;
    seq_t x = start;
    idx_t count = 1;          /* term 1 is the starting value */
    while (x >= start) {      /* keep iterating until we fall below */
        if (x & 1) {           /* odd */
            x = 3 * x + 1;
        } else {               /* even */
            x >>= 1;
        }
        ++count;
    }
    return (seq_t)count;
}

/*
 * A138978  Number of 3 X n matrices containing a 1 in the top left entry, all
 *          entries are integer values and adjacent entries differ by at most
 *          1.
 * https://oeis.org/A138978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138978: Number of 3×n matrices with a 1 in the top left entry,
   all entries integer and horizontally or vertically adjacent entries
   differ by at most 1.  The sequence satisfies
   a(n) = b(n)+c(n)+d(n) with initial values
      b(1)=1, c(1)=6, d(1)=2
   and recurrence
      b(n+1) = 3*b(n) + 2*c(n) + d(n)
      c(n+1) = 12*b(n) + 10*c(n) + 6*d(n)
      d(n+1) = 2*b(n) + 2*c(n) + 3*d(n)
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  Start from the first non‑empty column (n = 1).  */
    if (n == 0) {
        return (seq_t)0;   /* not defined in OEIS, but safe fallback   */
    }

    seq_t b = (seq_t)1;   /* b(1) */
    seq_t c = (seq_t)6;   /* c(1) */
    seq_t d = (seq_t)2;   /* d(1) */

    /* Compute b(n), c(n), d(n) by iterating the recurrence.  */
    for (idx_t i = 1; i < n; ++i) {
        seq_t nb = (seq_t)3 * b + (seq_t)2 * c + d;
        seq_t nc = (seq_t)12 * b + (seq_t)10 * c + (seq_t)6 * d;
        seq_t nd = (seq_t)2 * b + (seq_t)2 * c + (seq_t)3 * d;
        b = nb;
        c = nc;
        d = nd;
    }

    return b + c + d;
}

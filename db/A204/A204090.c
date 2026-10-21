/*
 * A204090  The number of 1 X n Haunted Mirror Maze puzzles with a unique
 *          solution where mirror orientation is fixed.
 * https://oeis.org/A204090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of OEIS sequence A204090
   Recurrence:
      a(0)=1, a(1)=2, a(2)=8, a(3)=34
      a(n) = 7*a(n-1) - 16*a(n-2) + 14*a(n-3) - 4*a(n-4)  for n ≥ 4
*/
#include <stddef.h>  /* for size_t (used only for internal casts) */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)34;

    /* Initialize the last four terms */
    seq_t prev4 = (seq_t)1;   /* a(0) */
    seq_t prev3 = (seq_t)2;   /* a(1) */
    seq_t prev2 = (seq_t)8;   /* a(2) */
    seq_t prev1 = (seq_t)34;  /* a(3) */
    seq_t cur = 0;

    /* Iterate until the desired n is reached */
    for (idx_t k = 4; k <= n; ++k) {
        cur = 7 * prev1 - 16 * prev2 + 14 * prev3 - 4 * prev4;
        /* Slide the window of last four terms */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

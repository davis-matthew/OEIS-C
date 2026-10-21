/*
 * A229314  Number of n X 1 0..3 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 2 0..3 array without adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * OEIS Sequence A229314
 *
 * Recurrence (for n ≥ 5):
 *   a(n) = 3*a(n-1) + 2*a(n-2) − a(n-3) + 2*a(n-4)
 * Initial values:
 *   a(1) = 4  a(2) = 14  a(3) = 50  a(4) = 176
 *
 * The implementation uses a simple iterative loop; it works
 * for any seq_t that supports the usual arithmetic operators.
 *********************************************************************/

#include <stdlib.h>

/* --------------------------------------------------------------- */
/* Main function for the sequence                                    */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly.                */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)50;
    if (n == 4) return (seq_t)176;

    /* For n ≥ 5 compute iteratively using the recurrence. */
    seq_t a1 = (seq_t)4;   /* a(n-4)   */
    seq_t a2 = (seq_t)14;  /* a(n-3)   */
    seq_t a3 = (seq_t)50;  /* a(n-2)   */
    seq_t a4 = (seq_t)176; /* a(n-1)   */
    seq_t ai;              /* a(n)     */

    /* idx_t is unsigned; loop is safe for n ≥ 5.          */
    for (idx_t i = 5; i <= n; ++i) {
        ai = 3 * a4 + 2 * a3 - a2 + 2 * a1; /* recurrence      */
        /* Shift the window for the next iteration.      */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}

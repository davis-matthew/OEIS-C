/*
 * A223337  5 X 5 X 5 triangular graph coloring a rectangular array: number of
 *          n X 1 0..14 arrays where 0..14 label nodes of a graph with edges
 *          0,1 0,2 1,2 1,3 1,4 2,4 3,4 2,5 4,5 3,6 3,7 4,7 6,7 4,8 5,8 7,8
 *          5,9 8,9 6,10 6,11 7,11 10,11 7,12 8,12 11,12 11,12 8,13 9,13 12,13
 *          9,14 13,14 and every array movemen...
 * https://oeis.org/A223337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - 8*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=15, a(2)=60, a(3)=264              */
/* This implementation evaluates the recurrence directly,  */
/* O(n) time, no dynamic memory allocation, suitable for   */
/* both ordinary integer and arbitrary‑precision types.     */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* undefined for n < 1; value returned is arbitrary */
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)60;
    if (n == 3) return (seq_t)264;
    if (n == 0) return (seq_t)0;   /* offset is 1, below it does not matter */

    /* keep the last three computed terms */
    seq_t a_nm3 = (seq_t)15;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)60;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)264;  /* a(n-1)   */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n = (seq_t)4 * a_nm1 + (seq_t)4 * a_nm2 - (seq_t)8 * a_nm3;
        /* shift the window forward */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;   /* a(n) */
}

/*
 * A158453  Number of connected spanning subgraphs in 3 X n grid.
 * https://oeis.org/A158453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (recurrence): a(n) = 22*a(n-1) - 73*a(n-2) + 54*a(n-3) - 8*a(n-4) 
 *   for n ≥ 5, with initial values a(1)=1, a(2)=23, a(3)=431, a(4)=7857.
 */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    /* base cases for the offset 1 sequence */
    if (n == 0) return (seq_t)0;          /* unused, keeps function well‑defined */
    if (n <= 4) {
        static const seq_t init[5] = {0, (seq_t)1, (seq_t)23, (seq_t)431, (seq_t)7857};
        return init[(size_t)n];
    }

    /* iterative computation using only the last four values */
    seq_t a1 = (seq_t)1;      /* a(1)  */
    seq_t a2 = (seq_t)23;     /* a(2)  */
    seq_t a3 = (seq_t)431;    /* a(3)  */
    seq_t a4 = (seq_t)7857;   /* a(4)  */
    seq_t an;

    for (idx_t i = 5; i <= n; ++i) {
        an = 22 * a4 - 73 * a3 + 54 * a2 - 8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;
}

/*
 * A230588  Number of n X 2 0..5 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 6, and upper left element zero.
 * https://oeis.org/A230588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A230588 – number of n×2 arrays over {0,…,5} with each element
 * adjacent (horizontally or vertically) to at least one element of
 * value (x+1) mod 6, and upper‑left element 0.
 *
 * Recurrence (valid for n≥5):
 *   a(n) = 5*a(n-1) - 5*a(n-2) + 5*a(n-3) + 8*a(n-4)
 * Initial values:
 *   a(1)=0 , a(2)=0 , a(3)=2 , a(4)=8
 *
 * The implementation computes the recurrence iteratively in O(n)
 * time per call and requires no dynamic allocation, so it works
 * correctly for both built‑in unsigned integer types and for an
 * arbitrary‑precision integer class compiled in C++17.
 */

#include <stdint.h>   /* for uint64_t / idx_t definition */

seq_t a(idx_t n)
{
    /* Offset starts at 1 – undefined for n<1, return 0 */
    if (n < 1)
        return 0;

    /* Handle the first four terms immediately */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 2;
    if (n == 4) return 8;

    /* Non‑local recurrence: keep the last four values */
    seq_t a1 = 0;  /* a(n-4)  */
    seq_t a2 = 0;  /* a(n-3)  */
    seq_t a3 = 2;  /* a(n-2)  */
    seq_t a4 = 8;  /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i) {
        /* temp = a4 - a3 + a2   (= a(n-1)-a(n-2)+a(n-3)) */
        seq_t temp = a4 - a3 + a2;
        /* a(n) = 5*temp + 8*a1   (since a1 = a(n-4)) */
        seq_t val = temp * 5 + a1 * 8;

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }

    /* a4 now holds a(n) */
    return a4;
}

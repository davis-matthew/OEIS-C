/*
 * A163704  Number of n X 2 binary arrays with all 1s connected, a path of 1s
 *          from left column to lower right corner, and no 1 having more than
 *          two 1s adjacent.
 * https://oeis.org/A163704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A163704
   Recurrence (for n ≥ 6):
   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4)
   Initial values:
   a(1) = 1,  a(2) = 5,  a(3) = 11,  a(4) = 21,  a(5) = 38
   Offset: 1
*/

#include <stddef.h>   /* size_t */

#define A163704_MAX_N 10000  /* reasonable upper bound for requested n */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range */

    /* Static cache for values up to A163704_MAX_N.
       values[0] is unused. */
    static seq_t values[A163704_MAX_N + 1];
    static idx_t cached_max = 0;  /* largest index that has been cached */

    /* Initialise cache with the first five terms once. */
    if (cached_max == 0) {
        values[1] = 1;
        values[2] = 5;
        values[3] = 11;
        values[4] = 21;
        values[5] = 38;
        cached_max = 5;
    }

    /* Case 1: requested n is within the cached range. */
    if (n <= cached_max) {
        return values[n];
    }

    /* Case 2: n is small enough to cache and compute iteratively. */
    if (n <= A163704_MAX_N) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) - a(i-3) + a(i-4) */
            values[i] = 3 * values[i - 1]
                      - 2 * values[i - 2]
                      -     values[i - 3]
                      +     values[i - 4];
        }
        cached_max = n;
        return values[n];
    }

    /* Case 3: n is larger than the cache size.
       Compute only the needed value with a rolling window. */
    seq_t a2 = 5;   /* a(2) */
    seq_t a3 = 11;  /* a(3) */
    seq_t a4 = 21;  /* a(4) */
    seq_t a5 = 38;  /* a(5) */

    seq_t current;  /* will hold a(i) */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 2*a(i-2) - a(i-3) + a(i-4) */
        current = 3 * a5
                - 2 * a4
                -     a3
                +     a2;

        /* shift window */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = current;
    }

    return current;   /* a(n) */
}

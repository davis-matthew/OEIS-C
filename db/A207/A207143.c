/*
 * A207143  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) for n > 9 Initial values
 *            (1‑based index, a(0) unused): a(1)=30, a(2)=48, a(3)=56,
 *            a(4)=95, a(5)=110, a(6)=187, a(7)=218, a(8)=371, a(9)=434
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 55, u64 n <= 119, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initVals[10] = {
    0,          /* a(0) – unused */
    30, 48, 56, 95, 110, 187, 218, 371, 434
};

seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined in OEIS; 0 is harmless */
        return 0;
    }
    if (n <= 9) {            /* return pre‑computed initial terms */
        return initVals[(size_t)n];
    }

    /* For n > 9 we use the empirical recurrence.
       The recurrence uses the three preceding terms:
       a(n) = a(n‑1) + 2*a(n‑2) - 2*a(n‑3)   for n > 9   */
    seq_t arr0 = initVals[7];   /* a(7) */
    seq_t arr1 = initVals[8];   /* a(8) */
    seq_t arr2 = initVals[9];   /* a(9) */
    seq_t cur  = 0;

    for (idx_t i = 10; i <= n; ++i) {
        /* a(i) = a(i‑1) + 2*a(i‑2) - 2*a(i‑3) */
        cur = arr2 + (arr1 + arr1) - (arr0 + arr0);
        /* Shift the sliding window for the next step */
        arr0 = arr1;
        arr1 = arr2;
        arr2 = cur;
    }
    return cur;
}

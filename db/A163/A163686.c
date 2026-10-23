/*
 * A163686  Number of n X 3 binary arrays with all 1s connected, a path of 1s
 *          from upper left corner to lower right corner, and no 1 having more
 *          than two 1s adjacent.
 * https://oeis.org/A163686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 58, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A163686 – number of n×3 binary arrays with all 1s connected, a path of 1s from upper left corner to lower right corner, and no 1 having more than two 1s adjacent. */

/* Recurrence (for n ≥ 13):
   a(n) = 5*a(n-1) -10*a(n-2)+10*a(n-3)- 2*a(n-4)-6*a(n-5)+6*a(n-6)-2*a(n-7)-a(n-8)+a(n-9)
   Initial terms for n = 1 … 12:
   1, 5, 17, 39, 83, 175, 375, 807, 1732, 3690, 7805, 16441
*/

#include <stddef.h>

static const seq_t init[12] = {
    1, 5, 17, 39, 83, 175, 375, 807, 1732, 3690, 7805, 16441
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* below offset – behavior irrelevant */
    if (n <= 12)
        return init[n - 1];

    /* sliding window of the last 9 values: win[0] = a(i‑9) ... win[8] = a(i‑1) */
    seq_t win[9];
    for (idx_t j = 0; j < 9; ++j)
        win[j] = init[3 + j];   /* init[3] = a(4), … , init[11] = a(12) */

    seq_t cur;
    for (idx_t i = 13; i <= n; ++i) {
        cur = win[8] * 5;
        cur -= win[7] * 10;
        cur += win[6] * 10;
        cur -= win[5] * 2;
        cur -= win[4] * 6;
        cur += win[3] * 6;
        cur -= win[2] * 2;
        cur -= win[1];
        cur += win[0];

        /* shift the window: win[k] = win[k+1] for k=0..7, win[8]=cur */
        for (int k = 0; k < 8; ++k)
            win[k] = win[k + 1];
        win[8] = cur;
    }
    return cur;
}

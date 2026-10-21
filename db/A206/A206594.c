/*
 * A206594  Number of (n+1)X2 0..3 arrays with every 2X3 or 3X2 subblock
 *          having no more than three equal edges, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A206594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A206594.
 * Base values (n = 1 .. 9):
 *   15, 171, 2532, 38810, 597829, 9217211, 142130380,
 *   2191712741, 33797293408
 * For n > 9:
 *   a(n) = 16*a(n-1) - 14*a(n-2) + 98*a(n-3) - 312*a(n-4)
 *          + 114*a(n-5) - 585*a(n-6) + 72*a(n-7) - 324*a(n-8) */

#include <stdlib.h>

static const seq_t BASE_VALUES[9] = {
    15ULL,
    171ULL,
    2532ULL,
    38810ULL,
    597829ULL,
    9217211ULL,
    142130380ULL,
    2191712741ULL,
    33797293408ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    if (n <= 9) return BASE_VALUES[n - 1];

    /* Sliding window of the last 8 values:
     *   last[0] = a(n-1), last[1] = a(n-2), ..., last[7] = a(n-8)   */
    seq_t last[8];
    last[0] = BASE_VALUES[8]; /* a9   */
    last[1] = BASE_VALUES[7]; /* a8   */
    last[2] = BASE_VALUES[6]; /* a7   */
    last[3] = BASE_VALUES[5]; /* a6   */
    last[4] = BASE_VALUES[4]; /* a5   */
    last[5] = BASE_VALUES[3]; /* a4   */
    last[6] = BASE_VALUES[2]; /* a3   */
    last[7] = BASE_VALUES[1]; /* a2   */

    seq_t new_val;
    for (idx_t i = 10; i <= n; ++i) {
        new_val = 16 * last[0] - 14 * last[1] + 98 * last[2]
                 - 312 * last[3] + 114 * last[4]
                 - 585 * last[5] + 72 * last[6]
                 - 324 * last[7];

        /* Shift the window: drop oldest, insert new */
        for (int j = 7; j > 0; --j) last[j] = last[j - 1];
        last[0] = new_val;
    }

    return last[0];
}

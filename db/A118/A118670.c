/*
 * A118670  Length of the shortest perfect modular pattern of type PMP(3,0) of
 *          n columns whose first row is 0...01 (with n-1 zeros).
 * https://oeis.org/A118670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the shortest perfect modular pattern of
 *            type PMP(3,0) with n columns whose first row is 0…01. The
 *            pattern is generated row by row using the linear recurrence
 *            derived from the modular‑pattern condition; the process stops
 *            when an all‑zero row would be produced. The returned va...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;                 /* number of columns */
    uint8_t *prev2 = (uint8_t *)malloc(N);
    uint8_t *prev1 = (uint8_t *)malloc(N);
    uint8_t *cur   = (uint8_t *)malloc(N);
    if (!prev2 || !prev1 || !cur) {
        free(prev2); free(prev1); free(cur);
        return 0;                       /* allocation failure */
    }

    /* row 0 is all zeros */
    memset(prev2, 0, N);

    /* row 1: 0 … 0 1 */
    memset(prev1, 0, N);
    if (N > 0) prev1[N-1] = 1;

    idx_t row_count = 1; /* number of rows generated so far */

    for (;;) {
        int all_zero = 1;
        for (size_t j = 0; j < N; ++j) {
            int sum = prev1[j] + prev2[j];
            if (j > 0)       sum += prev1[j-1];
            if (j + 1 < N)   sum += prev1[j+1];
            int mod = sum % 3;
            cur[j] = (uint8_t)((3 - mod) % 3);
            if (cur[j] != 0) all_zero = 0;
        }
        ++row_count;                /* cur is now the next row */

        if (all_zero) {             /* stopping condition */
            free(prev2); free(prev1); free(cur);
            return (seq_t)(row_count - 1); /* length excludes the all‑zero row */
        }

        /* rotate buffers for next iteration */
        uint8_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }
}

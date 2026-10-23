/*
 * A044838  Positive integers having more base-12 runs of even length than
 *          odd.
 * https://oeis.org/A044838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer having more base‑12 runs of
 *            even length than odd (searched sequentially)
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety; n≥1 in the sequence definition */

    idx_t count = 0;
    seq_t num = 0;

    /* Search numbers in ascending order until we find the nth qualifying integer */
    while (count < n) {
        ++num;                     /* start from 1 */

        /* Convert num to base‑12, storing digits in reverse order */
        seq_t temp = num;
        int digits[70];
        int len = 0;
        while (temp > 0) {
            digits[len++] = (int)(temp % 12);
            temp /= 12;
        }
        if (len == 0) {            /* never happens for positive numbers */
            digits[len++] = 0;
        }

        /* Count runs of identical digits, starting from the most significant digit */
        int prev = -1;
        int runlen = 0;
        int even = 0, odd = 0;

        for (int i = len - 1; i >= 0; --i) {
            int d = digits[i];
            if (prev == -1) {                 /* first digit */
                prev = d;
                runlen = 1;
            } else if (d == prev) {
                ++runlen;                     /* extend current run */
            } else {                         /* run finished */
                if (runlen % 2 == 0) ++even;
                else                 ++odd;
                prev = d;
                runlen = 1;
            }
        }
        /* Final run */
        if (runlen > 0) {
            if (runlen % 2 == 0) ++even;
            else                 ++odd;
        }

        if (even > odd) {                 /* number satisfies the property */
            ++count;
        }
    }

    return num;                           /* num is the nth term */
}

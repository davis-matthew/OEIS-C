/*
 * A031242  Length of n-th run of digit 0 in A031235.
 * https://oeis.org/A031242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the n-th run of digit 0 in the sequence
 *            A031235, which is obtained by concatenating the base‑5
 *            representations of the non‑negative integers. A representation
 *            is reversed when its zero‑run lengths form a non‑palindromic
 *            sequence
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of zero runs seen */

    for (idx_t m = 0;; ++m) {
        /* compute base‑5 digits of m (most significant first) */
        int digits[32];
        int len = 0;

        if (m == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            idx_t x = m;
            while (x > 0) {
                digits[len++] = (int)(x % 5);
                x /= 5;
            }
        }

        /* collect lengths of consecutive zero runs */
        int runs[32];
        int run_cnt = 0;
        int cur = 0;

        for (int i = len - 1; i >= 0; --i) {
            if (digits[i] == 0)
                cur++;
            else {
                if (cur > 0) {
                    runs[run_cnt++] = cur;
                    cur = 0;
                }
            }
        }
        if (cur > 0)
            runs[run_cnt++] = cur;

        /* no zero runs in this number – nothing to do */
        if (run_cnt == 0)
            continue;

        /* check if the run length sequence is a palindrome */
        bool pal = true;
        for (int i = 0; i < run_cnt / 2; ++i) {
            if (runs[i] != runs[run_cnt - 1 - i]) {
                pal = false;
                break;
            }
        }

        /* iterate through runs in the order they appear in the sequence */
        if (pal) {
            for (int i = 0; i < run_cnt; ++i) {
                ++count;
                if (count == n)
                    return (seq_t)runs[i];
            }
        } else {
            for (int i = run_cnt - 1; i >= 0; --i) {
                ++count;
                if (count == n)
                    return (seq_t)runs[i];
            }
        }
    }
}

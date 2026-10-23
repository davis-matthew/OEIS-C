/*
 * A146122  Bell numbers (A000110) read mod 32.
 * https://oeis.org/A146122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell number B(n) modulo 32 (period 96)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t bell[96];
    static bool built = false;

    if (!built) {
        seq_t prev[96];
        seq_t curr[96];
        int prev_len = 1;

        /* initial row (n = 0) */
        prev[0] = 1;
        bell[0] = 1;

        /* build rows 1 … 95 of the Bell triangle */
        for (int i = 1; i < 96; ++i) {
            int curr_len = i + 1;
            curr[0] = prev[prev_len - 1];
            curr[0] %= 32;
            for (int j = 1; j < curr_len; ++j) {
                curr[j] = (curr[j - 1] + prev[j - 1]) % 32;
            }
            bell[i] = curr[0] % 32;

            /* copy current row to become the previous row for next step */
            for (int k = 0; k < curr_len; ++k) {
                prev[k] = curr[k];
            }
            prev_len = curr_len;
        }
        built = true;
    }

    return bell[(unsigned)(n % 96)];
}

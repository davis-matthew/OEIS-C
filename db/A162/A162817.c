/*
 * A162817  Positive numbers n such that 40*n/(40+n) are integers.
 * https://oeis.org/A162817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = d-40 for each divisor d of 1600 with d>40
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t arr[10];
    static bool init = false;

    if (!init) {
        int cnt = 0;
        for (uint64_t d = 1; d * d <= 1600; ++d) {
            if (1600 % d == 0) {
                if (d > 40) arr[cnt++] = (seq_t)(d - 40);
                uint64_t dd = 1600 / d;
                if (dd != d && dd > 40) arr[cnt++] = (seq_t)(dd - 40);
            }
        }
        /* sort ascending */
        for (int i = 0; i < cnt; ++i) {
            for (int j = i + 1; j < cnt; ++j) {
                if (arr[j] < arr[i]) {
                    seq_t t = arr[i];
                    arr[i] = arr[j];
                    arr[j] = t;
                }
            }
        }
        init = true;
    }

    if (n == 0) return 0;
    if (n <= 10) return arr[(size_t)(n - 1)];
    return 0;
}

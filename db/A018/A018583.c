/*
 * A018583  Divisors of 668.
 * https://oeis.org/A018583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 668 in ascending order; a(n) = 0 for
 *            n > 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t divs[64];
    static size_t cnt = 0;

    if (!init) {
        const idx_t N = 668;
        size_t i, j;
        cnt = 0;
        for (i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divs[cnt++] = (seq_t)i;
                j = N / i;
                if (j != i) {
                    divs[cnt++] = (seq_t)j;
                }
            }
        }
        /* simple insertion sort for the very small number of elements */
        for (i = 0; i < cnt; ++i) {
            size_t k = i;
            for (j = i + 1; j < cnt; ++j) {
                if (divs[j] < divs[k]) k = j;
            }
            if (k != i) {
                seq_t tmp = divs[i];
                divs[i] = divs[k];
                divs[k] = tmp;
            }
        }
        init = true;
    }

    if (n == 0) return 0;
    if (n <= cnt) return divs[n - 1];
    return 0;
}

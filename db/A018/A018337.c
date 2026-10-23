/*
 * A018337  Divisors of 212.
 * https://oeis.org/A018337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 212: a(n) = nth divisor of 212 in increasing order; 0 for n>6 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divs[6];
    static size_t count = 0;

    if (!initialized) {
        const uint64_t N = 212ULL;
        size_t pos = 0;
        for (uint64_t i = 1; i * i <= N; ++i) {
            if (N % i == 0) {
                divs[pos++] = (seq_t)i;
                uint64_t j = N / i;
                if (j != i) {
                    divs[pos++] = (seq_t)j;
                }
            }
        }
        /* sort ascending */
        for (size_t i = 0; i + 1 < pos; ++i) {
            for (size_t j = i + 1; j < pos; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        count = pos;
        initialized = true;
    }

    if (n == 0 || n > count) return (seq_t)0;
    return divs[n - 1];
}

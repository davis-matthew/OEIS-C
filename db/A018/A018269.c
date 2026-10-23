/*
 * A018269  Divisors of 68.
 * https://oeis.org/A018269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 68
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache the divisors of 68 after first call */
    static seq_t divisors[6];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        const int limit = 68;
        size_t c = 0;
        for (int i = 1; i * i <= limit; ++i) {
            if (limit % i == 0) {
                divisors[c++] = (seq_t)i;
                int j = limit / i;
                if (j != i) {
                    divisors[c++] = (seq_t)j;
                }
            }
        }
        /* sort ascending */
        for (size_t i = 0; i < c - 1; ++i) {
            for (size_t j = i + 1; j < c; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        count = c;
        initialized = true;
    }

    if (n <= (idx_t)count) {
        return divisors[n - 1];
    }
    return (seq_t)0;
}

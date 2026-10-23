/*
 * A018480  Divisors of 484.
 * https://oeis.org/A018480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 484 in ascending order
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[9];
    static bool init = false;
    if (!init) {
        size_t pos = 0;
        /* Generate all divisors of 2^2 * 11^2 */
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t val2 = 1;
            for (int i = 0; i < e2; ++i) val2 *= 2;
            for (int e11 = 0; e11 <= 2; ++e11) {
                seq_t val = val2;
                for (int j = 0; j < e11; ++j) val *= 11;
                divs[pos++] = val;
            }
        }
        /* Simple insertion sort to order the divisors */
        for (size_t i = 0; i < 9; ++i) {
            for (size_t j = i + 1; j < 9; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        init = true;
    }
    if (n == 0 || n > 9) {
        return 0;
    }
    return divs[n - 1];
}

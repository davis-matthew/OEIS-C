/*
 * A018364  Divisors of 268.
 * https://oeis.org/A018364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 268 in ascending order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of divisors of 268 = 6 (2^2 * 67^1). */
    static seq_t divs[6];
    static bool done = false;
    if (!done) {
        size_t count = 0;
        /* Enumerate divisors from prime factorisation 2^2 * 67^1. */
        for (uint32_t e2 = 0; e2 <= 2; ++e2) {
            seq_t val2 = (seq_t)1;
            for (uint32_t i = 0; i < e2; ++i) val2 *= (seq_t)2;
            for (uint32_t e67 = 0; e67 <= 1; ++e67) {
                seq_t val = val2;
                for (uint32_t j = 0; j < e67; ++j) val *= (seq_t)67;
                divs[count++] = val;
            }
        }
        /* Simple insertion sort for at most 6 elements. */
        for (size_t i = 1; i < 6; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        done = true;
    }
    if (n == 0 || n > 6) return (seq_t)0;
    return divs[(size_t)(n - 1)];
}

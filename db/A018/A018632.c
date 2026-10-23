/*
 * A018632  Divisors of 762.
 * https://oeis.org/A018632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 762 (all divisors sorted increasingly)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divs[8];
    static bool init = false;

    if (!init) {
        /* compute all divisors of 762 = 2 * 3 * 127 */
        const seq_t primes[3] = {(seq_t)2, (seq_t)3, (seq_t)127};
        idx_t idx = 0;
        for (uint8_t mask = 0; mask < 8; ++mask) {
            seq_t d = (seq_t)1;
            for (int i = 0; i < 3; ++i)
                if (mask & (1U << i))
                    d *= primes[i];
            divs[idx++] = d;
        }
        /* insertion sort the 8 elements */
        for (idx = 1; idx < 8; ++idx) {
            seq_t key = divs[idx];
            int j = (int)idx - 1;
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }
        init = true;
    }

    if (n == 0 || n > 8)
        return (seq_t)0;
    return divs[(size_t)(n - 1)];
}

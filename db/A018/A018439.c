/*
 * A018439  Divisors of 410.
 * https://oeis.org/A018439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 410 in ascending order
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 410 is the fixed number whose divisors are enumerated. */
    const seq_t TARGET = (seq_t)410;
    static seq_t divisors[128];
    static size_t num_div = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Find all divisors of TARGET. */
        idx_t i;
        for (i = 1; (uint64_t)i * i <= (uint64_t)TARGET; ++i) {
            if ((uint64_t)TARGET % i == 0) {
                divisors[num_div++] = (seq_t)i;
                if (i != TARGET / i)  /* avoid double count of a square root */
                    divisors[num_div++] = (seq_t)(TARGET / i);
            }
        }

        /* Simple insertion sort for the small number of divisors. */
        for (size_t i = 1; i < num_div; ++i) {
            for (size_t j = i; j > 0 && divisors[j - 1] > divisors[j]; --j) {
                seq_t tmp = divisors[j];
                divisors[j] = divisors[j - 1];
                divisors[j - 1] = tmp;
            }
        }

        initialized = 1;
    }

    if (n < 1 || (idx_t)(n - 1) >= num_div)
        return (seq_t)0;   /* undefined for indices beyond the divisor list */

    return divisors[n - 1];
}

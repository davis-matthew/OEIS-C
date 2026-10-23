/*
 * A018307  Divisors of 153.
 * https://oeis.org/A018307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 153 (divisors sorted in ascending order)
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
    /* The number whose divisors we list */
    const seq_t LIMIT = (seq_t)153;

    /* Static cache: divisors of LIMIT, sorted ascending */
    static seq_t divisors[6];
    static size_t divisor_count = 0;
    static bool initialized = false;

    /* Initialise only once */
    if (!initialized) {
        divisor_count = 0;
        /* Enumerate all divisors up to sqrt(LIMIT) */
        for (seq_t d = (seq_t)1; d * d <= LIMIT; ++d) {
            if (LIMIT % d == 0) {
                divisors[divisor_count++] = d;
                if (d != LIMIT / d) {
                    divisors[divisor_count++] = LIMIT / d;
                }
            }
        }
        /* Simple selection sort (divisor_count ≤ 6) */
        for (size_t i = 0; i < divisor_count; ++i) {
            for (size_t j = i + 1; j < divisor_count; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        initialized = true;
    }

    /* Return the nth divisor if it exists, else 0 */
    if (n >= (idx_t)1 && n <= (idx_t)divisor_count) {
        return divisors[(size_t)(n - (idx_t)1)];
    }
    return (seq_t)0;
}

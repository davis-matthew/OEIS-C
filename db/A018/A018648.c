/*
 * A018648  Divisors of 790.
 * https://oeis.org/A018648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 790 (sorted ascending)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numbers 1,2,5,10,79,158,395,790 for n=1..8. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t divisors[128];
    static idx_t divisor_count = 0;
    static bool init = false;
    const seq_t target = 790;

    if (!init) {
        /* Find all divisors of 790 up to sqrt(790) */
        for (idx_t i = 1; (seq_t)i * (seq_t)i <= target; ++i) {
            if (target % (seq_t)i == 0) {
                divisors[divisor_count++] = (seq_t)i;
                if (i != target / (seq_t)i) {
                    divisors[divisor_count++] = target / (seq_t)i;
                }
            }
        }
        /* Sort the divisors in ascending order (n <= 8, so O(n^2) is fine) */
        for (idx_t i = 0; i < divisor_count; ++i) {
            for (idx_t j = i + 1; j < divisor_count; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        init = true;
    }

    if (n >= 1 && n <= divisor_count) {
        return divisors[n - 1];
    } else {
        /* Value undefined for n > number of divisors */
        return (seq_t)0;
    }
}

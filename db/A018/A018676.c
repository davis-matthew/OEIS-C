/*
 * A018676  Divisors of 840.
 * https://oeis.org/A018676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Divisors of 840              */
/*   a(n) = n-th smallest divisor of 840   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[32];
    static size_t div_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Compute all divisors of 840 in ascending order (they
         * are obtained automatically because we iterate i upwards). */
        for (uint64_t i = 1; i <= 840; ++i) {
            if (840 % i == 0)
                divisors[div_count++] = (seq_t)i;
        }
        initialized = true;
    }

    /* n is 1‑based.  For indices outside the valid range we return 0. */
    if (n < 1 || n > (idx_t)div_count)
        return (seq_t)0;

    return divisors[(size_t)(n - 1)];
}

/*
 * A083889  Number of divisors of n with largest digit = 2 (base 10).
 * https://oeis.org/A083889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose decimal representation has
 *            largest digit equal to 2
 * Range:     u8 n <= 9123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_digit_is_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/largest_digit_is_two.h"

/* Main sequence function.  External linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, but guard against n<1 */

    seq_t count = 0;
    /* Enumerate divisors by trial division up to sqrt(n). */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            if (largest_digit_is_two(d1)) count++;

            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                if (largest_digit_is_two(d2)) count++;
            }
        }
    }
    return count;
}

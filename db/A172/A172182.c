/*
 * A172182  Nonprimes of the form 6k + 1 or 6k + 2.
 * https://oeis.org/A172182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth nonprime of the form 6k+1 or 6k+2 (offset 1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, guard */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        seq_t val1 = (seq_t)k * 6 + 1;
        if (val1 >= 1) { /* 1 is included, 1 is not prime */
            if (val1 == 1 || !is_prime_big(val1)) {
                ++count;
                if (count == n) return val1;
            }
        }

        seq_t val2 = (seq_t)k * 6 + 2;
        if (val2 >= 2) {
            if (!is_prime_big(val2)) {
                ++count;
                if (count == n) return val2;
            }
        }

        ++k;
    }
}

/*
 * A164028  The n-th composite minus 3.
 * https://oeis.org/A164028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002808(n) - 3 (nth composite number minus 3)
 * Range:     u8 n <= 202, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t num = 4; ; ++num) {
        if (!is_prime(num)) {
            ++count;
            if (count == n)
                return (seq_t)(num - 3);
        }
    }
}

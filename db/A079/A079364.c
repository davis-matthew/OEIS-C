/*
 * A079364  Composite numbers having two composite neighbors.
 * https://oeis.org/A079364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number that has both neighboring
 *            integers composite
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t m = 4; ; ++m) {
        if (is_prime(m))
            continue;
        if (!is_prime(m - 1) && !is_prime(m + 1)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}

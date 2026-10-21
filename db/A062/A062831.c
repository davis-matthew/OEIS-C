/*
 * A062831  Number of ways n can be expressed as the sum of a nonzero square
 *          and 1 or a prime.
 * https://oeis.org/A062831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = a^2 + b with a>0 and b = 1 or
 *            b is prime
 * Range:     u8 n <= 64, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t count = 0;
    seq_t max_a = 0;
    if (n > 1) {
        max_a = floor_sqrt((seq_t)(n - 1));
    }
    for (seq_t a_val = 1; a_val <= max_a; ++a_val) {
        seq_t b = nn - a_val * a_val;
        if (b == 1) {
            count++;
        } else if (is_prime((idx_t)b)) {
            count++;
        }
    }
    return count;
}

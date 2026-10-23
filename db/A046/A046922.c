/*
 * A046922  Number of ways to express n as p+2a^2; p prime, a >= 0.
 * https://oeis.org/A046922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = p + 2*a^2 with p prime, a ≥
 *            0
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count representations for the given n */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* outside of offset, return 0 */
    seq_t count = 0;

    for (idx_t a_val = 0;; ++a_val) {
        seq_t t = (seq_t)2 * (seq_t)a_val * (seq_t)a_val;
        if (t > n) break;
        idx_t p = n - (idx_t)t;
        if (is_prime(p))
            ++count;
    }

    return count;
}

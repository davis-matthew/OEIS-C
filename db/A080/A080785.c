/*
 * A080785  Least p-smooth number not less than n, where p is the smallest
 *          prime factor of n.
 * https://oeis.org/A080785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least integer m >= n whose prime factors are all <=
 *            the smallest prime factor of n
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_p_smooth, _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_is_p_smooth.h"
#include "../utils/_smallest_prime_factor.h"

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;          /* a(1) = 1 by definition */

    uint64_t nn = (uint64_t)n;
    uint64_t p  = _smallest_prime_factor(nn);

    if (p == nn)  /* n itself is prime */
        return (seq_t)n;

    uint64_t m = nn;
    while (1) {
        if (_is_p_smooth(m, p))
            return (seq_t)m;
        ++m;
    }
}

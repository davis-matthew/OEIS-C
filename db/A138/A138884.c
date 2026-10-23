/*
 * A138884  Numbers that are not even superperfect numbers.
 * https://oeis.org/A138884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer that is NOT an even superperfect
 *            number (even superperfect numbers are of the form 2^(p-1) where
 *            p is prime and 2^p-1 is also prime)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"

static inline bool is_even_superperfect(seq_t m)
{
    if (m <= 1)
        return false;

    if ((m & (m - 1)) != 0)
        return false;

    idx_t k = 0;
    seq_t t = m;
    while (t > 1) {
        t >>= 1;
        ++k;
    }

    idx_t p = k + 1;
    if (!is_prime(p))
        return false;

    seq_t mers = pow2(p) - 1;
    return is_prime_big(mers);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, but harmless */

    idx_t seen = 0;
    seq_t current = 0;

    while (seen < n) {
        ++current;
        if (!is_even_superperfect(current))
            ++seen;
    }
    return current;
}

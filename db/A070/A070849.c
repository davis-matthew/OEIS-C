/*
 * A070849  Smallest prime == 1 mod (5n).
 * https://oeis.org/A070849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 610, u32 n <= 1798, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for smallest prime ≡ 1 (mod 5*n) */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* step = 5*n, computed in seq_t to avoid overflow on idx_t */
    seq_t step = (seq_t)n * (seq_t)5;
    idx_t m = 1;

    while (true) {
        seq_t p = (seq_t)1 + (seq_t)m * step;
        if (is_prime_big(p))
            return p;
        ++m; /* increment quotient until a prime is found */
    }
}

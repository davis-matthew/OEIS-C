/*
 * A201788  Primes of the form 5n^2 - 7.
 * https://oeis.org/A201788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   miller_rabin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201788:  the nth prime of the form 5k^2-7.
   a(n) = the n-th prime p such that p = 5*k^2 - 7 for some integer k ≥ 1. */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/miller_rabin.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1 – undefined below 1, return 0 as safe default */
        return 0;

    idx_t found = 0;            /* number of primes seen so far */
    seq_t k = 1;                /* current k to test */

    while (found < n) {
        seq_t p = 5 * k * k - 7;   /* compute candidate; fits in seq_t */
        /* p is guaranteed to be < 2^64 for all practical n (n ≈ 10^4) */
        if (miller_rabin((uint64_t)p)) {
            ++found;
            if (found == n)
                return p;
        }
        ++k;
    }
    /* unreachable */
    return 0;
}

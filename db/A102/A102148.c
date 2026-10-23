/*
 * A102148  Numbers k such that 101*k + 11 is prime.
 * https://oeis.org/A102148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 101*k + 11 is prime. */
/* (OEIS A102148) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* Count how many valid k have been found so far. */
    idx_t count = 0;
    idx_t k = 0;          /* candidate k */

    while (true)
    {
        uint64_t val = 101ULL * k + 11ULL;   /* 101*k + 11 */

        if (is_prime_uint64(val))
        {
            ++count;
            if (count == n)
                return (seq_t)k;            /* k fits in seq_t */
        }
        ++k;
    }

    /* When n is out of range this point is never reached. */
    abort();
}

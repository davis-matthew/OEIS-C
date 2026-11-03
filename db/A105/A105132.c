/*
 * A105132  Primes of the form 1024n + 513.
 * https://oeis.org/A105132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the primes congruent to 513 modulo 1024 (i.e. of the
 *            form 1024*k + 513)
 * Range:     u8 none, u16 n <= 10, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 8222 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Usage: a(1) = 7681, a(2) = 10753, … */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (1)
    {
        /* val = 1024*k + 513 fits into 64 bits for all n needed by the tests */
        uint64_t val = 1024ULL * k + 513ULL;
        if (is_prime_u64(val))
        {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
        ++k;
    }
    /* Should never be reached */
    return (seq_t)0;
}

/*
 * A105132  Primes of the form 1024n + 513.
 * https://oeis.org/A105132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the primes congruent to 513 modulo 1024 (i.e. of the
 *            form 1024*k + 513)
 * Range:     u8 none, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Usage: a(1) = 7681, a(2) = 10753, … */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (1)
    {
        /* val = 1024*k + 513 fits into 64 bits for all n needed by the tests */
        uint64_t val = 1024ULL * k + 513ULL;
        if (is_prime(val))
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

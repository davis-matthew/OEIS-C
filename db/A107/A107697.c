/*
 * A107697  Primes with digital product = 12.
 * https://oeis.org/A107697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 86 (2026-11-03)
 * Helpers:   digit_product_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107697: primes whose decimal digit product equals 12 (search) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/digit_product_seq.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* Cached list of primes whose digit product is 12. */
    static uint64_t *primes        = NULL;
    static size_t  primes_cap      = 0;
    static size_t  primes_sz       = 0;
    static uint64_t next_candidate = 2;

    /* Grow the cache until it contains at least n primes. */
    while (primes_sz < (size_t)n)
    {
        if (is_prime_u64(next_candidate))
        {
            if (digit_product_seq((seq_t)next_candidate) == 12)
            {
                if (primes_sz == primes_cap)
                {
                    size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);          /* Out of memory. */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_sz++] = next_candidate;
            }
        }
        ++next_candidate;
    }

    return (seq_t)primes[n - 1];
}

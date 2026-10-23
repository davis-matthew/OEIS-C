/*
 * A140969  Prime numbers whose hexadecimal representation uses only the
 *          digits A,B,C,D,E,F (and not the decimal digits).
 * https://oeis.org/A140969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime whose hexadecimal representation uses only
 *            the digits A,B,C,D,E,F (i.e. all nibbles are ≥ 10)
 * Range:     u8 n <= 7, u16 n <= 128, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hex_allowed, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/hex_allowed.h"

/* A fixed-size cache large enough for n ≈ 10000.
   The 10000th such prime is < 2³⁵, so it comfortably fits in seq_t. */
#define CACHE_CAP 20000
static seq_t prime_cache[CACHE_CAP];
static size_t cache_len   = 0;   /* number of cached primes */
static idx_t  last_tested = 0;   /* last integer examined */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* offset is 1 – value for n==0 is irrelevant */

    /* Return a cached value if already known. */
    if (n <= cache_len)
        return prime_cache[n - 1];

    /* Search forward until we collect the n‑th prime. */
    idx_t cur = last_tested + 1;
    while (1)
    {
        if (hex_allowed(cur) && is_prime_big((seq_t)cur))
        {
            if (cache_len < CACHE_CAP)
                prime_cache[cache_len++] = (seq_t)cur;
            last_tested = cur;
            if (n <= cache_len)
                return prime_cache[n - 1];
        }
        ++cur;
    }
}

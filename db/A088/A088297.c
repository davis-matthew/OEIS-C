/*
 * A088297  Primes in which no digit is coprime to its neighbors.
 * https://oeis.org/A088297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, is_special_prime, uint_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates terms of OEIS A088297: primes in which no digit is
   coprime to its neighbors and no digit is 0 or 1.  The function
   returns the n-th such prime, with a(1)=2. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/is_special_prime.h"
#include "../utils/uint_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset 0 not used */

    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;
    static seq_t next_candidate  = 2;   /* next integer to test */

    /* Grow cache until it contains n primes of the desired type. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        /* Advance to the next odd (or 3 after 2) candidate. */
        if (cand <= (seq_t)2)
            next_candidate = (seq_t)3;
        else
            next_candidate = cand + (seq_t)2;

        if (!is_prime_big(cand))
            continue;          /* must be prime */

        if (is_special_prime(cand))
            append_candidate(&cache, &cache_len, &cache_cap, cand);
    }

    return cache[(size_t)n - 1];
}

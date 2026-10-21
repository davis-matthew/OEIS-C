/*
 * A214888  Primes congruent to {2, 3} mod 11.
 * https://oeis.org/A214888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n‑th prime that is ≡ 2 or ≡ 3 (mod 11)
 * Range:     u8 n <= 13, u16 n <= 995, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for A214888 – primes congruent to 2 or 3 modulo 11.
   The function keeps a static cache of the primes seen so far so that
   repeated calls are fast.  The cache grows on demand.
   The implementation works for any seq_t provided by the build
   (uint8_t …, unsigned __int128, or arbitrary‑precision integer classes). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* static cache of primes ≡ 2 or 3 (mod 11) */
static seq_t *prime_cache     = NULL;
static size_t prime_cache_cap  = 0;
static size_t prime_cache_len  = 0;
static idx_t  next_candidate   = 2;   /* start from the first integer */

seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined offset – return 0 */
        return (seq_t)0;

    /* make sure the cache holds at least n primes */
    while (prime_cache_len < n) {
        /* search for the next qualifying prime */
        for ( ; ; ++next_candidate) {
            /* quick skip numbers not ≡ 2 or 3 (mod 11) */
            uint64_t r = next_candidate % 11;
            if (r != 2 && r != 3)
                continue;

            /* primality test for arbitrary precision */
            if (is_prime_big((seq_t)next_candidate)) {
                /* grow cache if necessary */
                grow_seq(&prime_cache, &prime_cache_cap,
                         prime_cache_len + 1);
                /* store the prime */
                prime_cache[prime_cache_len++] = (seq_t)next_candidate;
                /* advance to the next candidate for future searches */
                ++next_candidate;
                break;                    /* found next term */
            }
        }
    }

    return prime_cache[n - 1];
}

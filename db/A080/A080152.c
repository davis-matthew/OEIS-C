/*
 * A080152  Values of n corresponding to the primes given in A073640. The
 *          concatenation of the a(n) and a(n+1)-th primes for any n is also
 *          prime.
 * https://oeis.org/A080152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, append_candidate, concat_parts, is_prime_big,
 *            next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A080152 – Values of n corresponding to the primes given in A073640. 
 *  The concatenation of the a(n)-th and a(n+1)-th primes is always prime.
 *  a(1) = 1, a(n) is the smallest integer > a(n-1) for which
 *      concat( prime[a(n-1)], prime[a(n)] ) is prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL; /* a(1), a(2), ... */
static size_t cache_len = 0, cache_cap = 0;

static uint64_t *primes = NULL;  /* 1‑based prime list, stored 0‑based */
static size_t primes_len = 0, primes_cap = 0;
static uint64_t last_prime = 0;

/* initialise static data */
static void init(void)
{
    if (cache != NULL)
        return;                /* already initialised */

    /* sequence cache – start with a(1)=1 */
    cache_cap = 8;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    cache[0] = (seq_t)1;
    cache_len = 1;

    /* prime list – start with 2 */
    primes_cap = 8;
    primes = (uint64_t *)malloc(sizeof(uint64_t) * primes_cap);
    primes[0] = 2;
    primes_len = 1;
    last_prime = 2;
}

/* return a(n) (n >= 1).  For n < 1 returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    init();

    /* generate sequence terms up to index n */
    while (cache_len < (size_t)n) {
        /* current last element of the sequence */
        idx_t last = (idx_t)cache[cache_len - 1];

        /* search for next index that satisfies the concatenation property */
        idx_t j = last + 1;
        for (;; ++j) {
            /* make sure we know the j‑th prime */
            while (primes_len <= (size_t)j) {
                uint64_t p = next_prime(last_prime);
                last_prime = p;

                if (primes_len == primes_cap) {
                    primes_cap *= 2;
                    primes = (uint64_t *)realloc(primes,
                                                sizeof(uint64_t) * primes_cap);
                }
                primes[primes_len++] = p;
            }

            /* concat(prime[last], prime[j]) */
            seq_t concat = concat_parts(primes[last - 1], primes[j - 1]);

            if (is_prime_big(concat)) {
                /* found next element of the sequence */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache,
                                             sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = (seq_t)j;
                break;
            }
        }
    }

    return cache[n - 1];
}

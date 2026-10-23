/*
 * A134735  Primes followed by the difference from the next prime.
 * https://oeis.org/A134735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   if n is odd: a(n)=A000040((n+1)/2) else: a(n)=A001223(n/2)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

static void ensure_prime_count(idx_t needed)
{
    if ((size_t)needed <= prime_len) return;
    /* grow cache to required number of primes */
    ensure_prime_capacity((size_t)needed);
    for (size_t i = prime_len; i < (size_t)needed; ++i) {
        prime_cache[i] = (seq_t)get_prime((idx_t)(i + 1));
    }
    prime_len = (size_t)needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, guard against misuse */

    if ((n & 1) == 1) {                /* odd n: return nth prime */
        idx_t k = (n + 1) / 2;
        ensure_prime_count(k);
        return prime_cache[(size_t)(k - 1)];
    } else {                            /* even n: difference of consecutive primes */
        idx_t k   = n / 2;              /* we need primes up to index k+1 */
        idx_t need = k + 1;
        ensure_prime_count(need);
        return prime_cache[(size_t)k] - prime_cache[(size_t)(k - 1)];
    }
}

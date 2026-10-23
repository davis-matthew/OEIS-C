/*
 * A027864  Primes of the form k^2 + (k+1)^2 + (k+2)^2 = 3*(k+1)^2+2.
 * https://oeis.org/A027864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 3*(k+1)^2 + 2, enumerated by increasing k.
   a(1) = 5, a(2) = 29, a(3) = 149, ... */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* cache of found primes */
static seq_t *prime_cache      = NULL;
static size_t prime_cache_cap  = 0;
static idx_t  prime_cache_cnt  = 0;
static idx_t  last_k           = 0;   /* last k that was tested */

/* ensure the cache can hold 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= prime_cache_cap) return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside of offset; harmless */

    while (prime_cache_cnt < n)
    {
        /* test current k = last_k */
        uint64_t kp1   = (uint64_t)(last_k + 1);          /* k+1 */
        uint64_t val64 = 3 * kp1 * kp1 + 2;               /* 3*(k+1)^2 + 2 */
        seq_t  val     = (seq_t)val64;                    /* cast to seq_t */

        if (is_prime_big(val))
        {
            ensure_cache(prime_cache_cnt + 1);
            prime_cache[prime_cache_cnt++] = val;
        }
        ++last_k;  /* advance to next k */
    }
    return prime_cache[n - 1];
}

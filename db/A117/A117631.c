/*
 * A117631  a(1)=433640083; a(n+1)= the largest prime factor of
 *          a(n)+b(n)+c(n), where a(n)<b(n)<c(n) and a(n),b(n) and c(n) are
 *          three consecutive primes.
 * https://oeis.org/A117631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gpf, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=433640083; a(n+1)=gpf(a(n)+next_prime(a(n))+next_prime(next_prime(a(n))) ); */
/* The sequence eventually enters the cycle 41,131,37,11. */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/gpf.h"          /* uint64_t gpf(uint64_t x);          */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t s); */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* bail out if allocation fails */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined index, return 0 as safe default */
    if (cache_len < n) {
        ensure_capacity((size_t)n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            uint64_t val;
            if (i == 1) {
                val = 433640083ULL;
            } else {
                uint64_t prev   = (uint64_t)cache[i - 2];
                uint64_t p1     = next_prime(prev);
                uint64_t p2     = next_prime(p1);
                uint64_t sum    = (uint64_t)prev + p1 + p2;
                val = gpf(sum);  /* largest prime factor of the sum */
            }
            cache[i - 1] = (seq_t)val;
        }
        cache_len = n;
    }
    return cache[n - 1];
}

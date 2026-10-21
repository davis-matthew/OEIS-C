/*
 * A084160  First occurrence prime gaps of the primes in sequence A002313
 *          (Real primes with corresponding complex primes). a(0) = 2 with
 *          length of gap 3. For n>0 the size of the gap in the sequence is
 *          4n, a(n) is the starting prime of the gap.
 * https://oeis.org/A084160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, for n>0 a(n) is the starting prime of the first *
 *            occurrence of a gap of size 4*n between consecutive primes in
 *            A002313 * (the sequence of primes 2 and primes ≡1 (mod 4))
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static uint64_t last_prime = 0;   /* previous prime in A002313 */
static bool initialized = false;

/* enlarge cache to hold index 'n' (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t need = (size_t)n + 1;
    size_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* out of memory */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* return next prime from A002313 greater than 'after' */
static uint64_t next_real_prime_u64(uint64_t after)
{
    uint64_t cand = next_prime(after + 1);
    /* 2 is the only even prime in A002313 */
    while (!(cand == 2ull || (cand & 3ull) == 1)) {
        cand = next_prime(cand + 1);
    }
    return cand;
}

/* compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!initialized) {
        ensure_cache_size(1);
        cache[0] = 0;
        cache[1] = 0;
        last_prime = 2;       /* start from the first prime in A002313 */
        initialized = true;
    }

    if (n >= (idx_t)cache_cap) ensure_cache_size(n);
    if (cache[n] != 0) return cache[n];

    while (cache[n] == 0) {
        uint64_t nextp = next_real_prime_u64(last_prime);
        seq_t gap = (seq_t)nextp - (seq_t)last_prime;

        if (gap == (seq_t)3) {
            if (cache[0] == 0) cache[0] = (seq_t)last_prime;
        } else if ((gap % (seq_t)4) == 0) {
            idx_t idx = (idx_t)(gap / (seq_t)4);
            if (idx < (idx_t)cache_cap) {
                if (cache[idx] == 0) {
                    cache[idx] = (seq_t)last_prime;
                    if (idx == n) return cache[n];
                }
            } else {
                ensure_cache_size(idx);
                if (cache[idx] == 0) {
                    cache[idx] = (seq_t)last_prime;
                    if (idx == n) return cache[n];
                }
            }
        }

        last_prime = nextp;
    }

    /* when loop exits, cache[n] has been set */
    return cache[n];
}

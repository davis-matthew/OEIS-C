/*
 * A095738  Numbers that are coprime to sigma but are not prime powers.
 * https://oeis.org/A095738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 5731, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9881 (2026-11-01)
 * Helpers:   gcd, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers that are coprime to σ(n) but are not prime powers
 * (solitary numbers).  The function builds the sequence incrementally
 * by testing successive integers starting at 21 and caching the valid
 * terms; this works for any seq_t / idx_t combination available in the
 * build.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/smallest_prime_factor.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;      /* array of cached sequence values */
static size_t cache_cap = 0;     /* current allocation size */
static idx_t cache_size = 0;     /* number of cached terms */

/* next integer candidate to examine (starts after the first offset 21) */
static idx_t next_candidate = 21;

/* Return true iff n is a prime power (including n being prime). */
static bool is_prime_power(uint64_t n)
{
    uint64_t p = smallest_prime_factor(n);
    if (p == 0) return false;        /* n < 2, not expected here */
    if (p == n) return true;         /* n is prime */

    /* divide out all copies of p */
    do {
        n /= p;
    } while (n % p == 0);

    return n == 1;
}

/* Expand the cache until it contains at least N terms. */
static void ensure_cache(idx_t N)
{
    while (cache_size < N) {
        seq_t sigma = sigma_of(next_candidate);
        seq_t g = gcd((seq_t)next_candidate, sigma);

        if (g == 1 && !is_prime_power((uint64_t)next_candidate)) {
            /* make room if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                if (cache_cap == 0) {
                    cache = seq_alloc(new_cap);
                } else {
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();    /* allocation failure  */
                    cache = tmp;
                }
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n==0, guard */
    ensure_cache(n);
    return cache[n - 1];
}

/*
 * A065377  Primes not of the form p + k^2, with p prime and k > 0.
 * https://oeis.org/A065377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) are the primes that cannot be written as p + k^2
   with p a prime and k>0.  We generate primes on demand,
   test each one, and cache the qualifying primes.  The
   algorithm uses only trial division and integer square
   roots, and works for any seq_t and idx_t size. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* square root of a 64‑bit integer */
static inline uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t r = (uint64_t) sqrt((long double) v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

/* static storage for primes and the sequence */
static uint64_t   *prime_list  = NULL;  /* the discovered primes   */
static size_t     prime_len   = 0;      /* number of primes stored */
static size_t     prime_cap   = 0;      /* allocated capacity      */

static seq_t      *cache_list  = NULL;  /* sequence values (primes) */
static size_t     cache_len   = 0;      /* number of terms cached */
static size_t     cache_cap   = 0;      /* allocated capacity      */

static uint64_t   last_checked = 1;     /* last integer tested for primality */

/* ensure prime list has room for at least n more elements */
static void grow_prime_list(size_t more)
{
    if (prime_len + more > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (prime_len + more > new_cap) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* ensure cache has room for at least n more elements */
static void grow_cache_list(size_t more)
{
    if (cache_len + more > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (cache_len + more > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache_list = tmp;
        cache_cap  = new_cap;
    }
}

/* primality test using the list of previously found primes */
static bool is_prime_uint64(uint64_t cand)
{
    if (cand < 2) return false;
    uint64_t r = isqrt_uint64(cand);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > r) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* the OEIS sequence function */
seq_t a(idx_t n)
{
    /* build sequence until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* find next prime */
        uint64_t q = 0;
        do {
            ++last_checked;
            if (is_prime_uint64(last_checked)) {
                q = last_checked;
                break;
            }
        } while (true);

        /* add the new prime to the list */
        grow_prime_list(1);
        prime_list[prime_len++] = q;

        /* check whether q can be expressed as p + k^2 */
        bool representable = false;
        for (size_t i = 0; i + 1 < prime_len; ++i) {   /* all primes < q */
            uint64_t p   = prime_list[i];
            uint64_t diff = q - p;
            if (diff == 0) continue;
            uint64_t r = isqrt_uint64(diff);
            if (r > 0 && r * r == diff) {               /* perfect square */
                representable = true;
                break;
            }
        }

        /* if not representable, store it in the cache */
        if (!representable) {
            grow_cache_list(1);
            cache_list[cache_len++] = (seq_t)q;
        }

        /* safety limit: we know the last term is 7549. */
        if (q > 8000 && cache_len < (size_t)n) continue;
        if (q > 8000 && cache_len >= (size_t)n) break;
    }

    if ((size_t)n <= cache_len)
        return cache_list[(size_t)n - 1];
    else
        return (seq_t)0;   /* sequence is finite; no value beyond known terms */
}

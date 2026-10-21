/*
 * A204911  The prime q>=5 such that n divides p-q, where p>q is the least
 *          prime for which such a prime q exists.
 * https://oeis.org/A204911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime q ≥ 5 such that n ∣ (p − q), where p>q is the
 *            least prime for which such a prime q exists. The function
 *            searches primes p in increasing order, keeping the most recent
 *            prime of each residue modulo n; when a residue repeats with a
 *            previously seen prime ≥ 5, that earlier prime is re...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t) -> bool */

/* ------------------------------------------------------------------ */
/* prime storage                                                  */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;          /* list of primes found so far     */
static size_t primes_cnt = 0;            /* number of primes stored         */
static size_t primes_cap = 0;            /* allocated capacity              */
static uint64_t next_candidate = 2;      /* next integer to test for prime */

static void grow_primes(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

static void ensure_primes(size_t cnt)
{
    while (primes_cnt < cnt) {
        while (!is_prime(next_candidate)) ++next_candidate;
        grow_primes(primes_cnt + 1);
        primes[primes_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* cache for results                                               */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_size = 0;              /* allocated cache size */

/* ------------------------------------------------------------------ */
/* main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for 0, return 0 */

    /* Expand cache to cover this index.  idx_t is unsigned, so
       conversion to size_t is safe for the allowed values (≤~10⁴). */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) abort();
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    if (cache[n] != 0)                    /* result already computed */
        return cache[n];

    /* Allocate storage for the most recent prime of each residue
       modulo n.  All entries start at 0 (sentinel). */
    seq_t *last = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    if (!last) abort();

    /* Iterate primes until we find a suitable pair.  The primes
       list is built lazily as needed. */
    size_t idx = 0;                       /* index into primes vector */
    while (true) {
        uint64_t p;

        if (idx < primes_cnt) {
            p = primes[idx];
        } else {
            /* Generate next prime on demand. */
            while (!is_prime(next_candidate)) ++next_candidate;
            grow_primes(primes_cnt + 1);
            primes[primes_cnt] = next_candidate;
            p = next_candidate;
            ++next_candidate;
            ++primes_cnt;
        }
        ++idx;

        uint64_t r = p % n;                /* residue class of p mod n */
        seq_t q_prev = last[r];            /* previous prime of same residue */

        if (q_prev >= 5) {                /* we have a valid earlier prime */
            cache[n] = q_prev;
            free(last);
            return q_prev;
        }

        /* Store this prime as the new "previous" for its residue. */
        last[r] = (seq_t)p;
    }
}

/*
 * A178563  Primes p such that A178344(p) is also prime.
 * https://oeis.org/A178563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178563:
   a(n) is the n‑th prime p>2 for which
   S(p) = Σ_{i=0}^{⌊log₂p⌋} ( (p>>i)&1 ? prime_i : 1 ) is also prime.
   Here prime_i is the i‑th prime (prime_0 = 2).  The sequence is
   produced by searching primes and testing S(p). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/append_candidate.h"   /* append_candidate */
#include "../utils/is_prime_small.h"     /* is_prime_small */

static const idx_t INITIAL_PRIME_LIMIT = 800000;   /* enough for n≈10 000 */

static bool init_done = false;
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_cnt   = 0;     /* number of primes */
static size_t processed_idx = 0;   /* next prime to test */

/* cache of qualifying primes */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t current_limit = INITIAL_PRIME_LIMIT;

/* compute S(p) for a prime p */
static seq_t sum_for_prime(idx_t p)
{
    seq_t sum = 0;
    idx_t bit   = 0;
    idx_t temp  = p;
    while (temp != 0) {
        if (temp & 1)
            sum += (seq_t)prime_list[bit];   /* prime_i */
        else
            sum += 1;                       /* 1 for a 0‑bit */
        temp >>= 1;
        ++bit;
    }
    return sum;
}

/* extend the prime table if we have exhausted it */
static void extend_primes(void)
{
    idx_t new_limit = current_limit * 2;
    free(prime_list);
    build_primes_upto(new_limit, (idx_t **)&prime_list, &prime_cnt);
    processed_idx = 0;
    current_limit = new_limit;
}

/* ensure that cache contains at least n terms */
static void ensure_cache(size_t n)
{
    if (!init_done) {
        build_primes_upto(current_limit, (idx_t **)&prime_list, &prime_cnt);
        init_done = true;
    }

    while (cache_cnt < n) {
        if (processed_idx >= prime_cnt)
            extend_primes();

        idx_t p = prime_list[processed_idx++];
        if (p <= 2)      /* skip 2; sequence starts at 3 */
            continue;

        seq_t s = sum_for_prime(p);
        if (is_prime_small(s))
            append_candidate(&cache, &cache_cnt, &cache_cap, (seq_t)p);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* no defined value for 0 */
    ensure_cache((size_t)n);
    return cache[n - 1];
}

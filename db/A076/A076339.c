/*
 * A076339  Primes of the form 512*k+1.
 * https://oeis.org/A076339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 25, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A076339 – primes of the form 512*k + 1
   Generation method: incremental enumeration of candidates
   (512*k+1) and testing for primality with the supplied
   big‑integer primality routine is_prime_big.
   The implementation keeps a static cache so that
   repeated calls (in any order) are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* static cache data */
static seq_t *prime_cache = NULL;    /* array of found primes   */
static size_t   cache_cap  = 0;      /* allocated capacity     */
static size_t   cache_cnt  = 0;      /* number of primes stored*/
static idx_t   next_k      = 0;      /* next k value (512*k+1) */

/* enlarge the cache array if needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();              /* unrecoverable memory failure */
    prime_cache = tmp;
    cache_cap   = newcap;
}

/* ensure that at least 'n' primes of the required form are cached */
static void ensure_cache(idx_t n)
{
    if (cache_cnt >= (size_t)n) return;

    while (cache_cnt < (size_t)n) {
        seq_t cand = ((seq_t)512) * (seq_t)next_k + (seq_t)1;
        ++next_k;
        /* skip the trivial non‑prime 1 */
        if (cand <= (seq_t)1) continue;
        if (is_prime_big(cand)) {
            grow_cache(cache_cnt + 1);
            prime_cache[cache_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of range – not used by OEIS */
    ensure_cache(n);
    return prime_cache[n - 1];
}

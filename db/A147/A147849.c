/*
 * A147849  a(n) is the smallest triangular number > n-th prime.
 * https://oeis.org/A147849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest triangular number > n‑th prime
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

static uint64_t *primes_cache = NULL;   /* array of primes in ascending order */
static idx_t     primes_cnt   = 0;      /* number of primes currently stored */
static idx_t     primes_limit = 0;      /* sieve limit that produced them */

/* Ensure that at least `n` primes are available. */
static void
ensure_primes(idx_t n)
{
    if (n == 0) return;
    if (primes_cnt >= n) return;

    /* Initial upper bound for the n‑th prime (valid for n up to ~10 000). */
    idx_t limit = n * 15 + 100;
    if (limit < 20) limit = 20;

    /* If the current sieve covers less than the needed limit, rebuild it. */
    if (primes_limit < limit) {
        free(primes_cache);
        build_primes_upto(limit, &primes_cache, &primes_cnt);
        primes_limit = limit;
    } else {
        /* We have enough sieve range but not enough primes: extend. */
        while (primes_cnt < n) {
            limit *= 2;
            free(primes_cache);
            build_primes_upto(limit, &primes_cache, &primes_cnt);
            primes_limit = limit;
        }
    }
}

/* Return the smallest triangular number greater than the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for indices < 1 */

    /* Obtain the n‑th prime. */
    ensure_primes(n);
    uint64_t prime = primes_cache[(size_t)n - 1];  /* primes_cache is 0‑based */

    /* Compute k = ⌊(√(8*prime+1) – 1) / 2⌋ + 1, guaranteeing k*(k+1)/2 > prime. */
    seq_t delta = (seq_t)prime * (seq_t)8 + (seq_t)1;
    seq_t sqrt_val = floor_sqrt(delta);
    idx_t k = ((idx_t)sqrt_val - 1) / 2 + 1;

    seq_t tri = (seq_t)k * (seq_t)(k + 1) / (seq_t)2;

    /* Safety guard: increment until tri > prime. */
    while (tri <= (seq_t)prime) {
        ++k;
        tri = (seq_t)k * (seq_t)(k + 1) / (seq_t)2;
    }

    return tri;
}

/*
 * A091803  (Smallest n-digit multiple of the n-th prime) - 10^(n-1).
 * https://oeis.org/A091803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   powmod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091803
   a(n) = (smallest n‑digit multiple of the n‑th prime) – 10^(n‑1)
   This is equivalent to a(n) = (p_n – (10^(n‑1) mod p_n)) mod p_n
   where p_n is the n‑th prime.  The result is always smaller than p_n
   and therefore fits in a 32‑bit unsigned integer.  The implementation
   generates the first n primes by a simple sieve and then computes the
   modular power 10^(n‑1) mod p_n.  All work is done with 32‑bit integers
   and 64‑bit intermediates to avoid overflow.  The function works for
   any n ≥ 1 and is independent of the order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   /* for log() (allowed for standard builds) */
#include "../utils/powmod10.h"

/* global prime cache ----------------------------------------------------- */
static uint32_t *prime_cache = NULL;    /* array of primes, 0‑indexed */
static size_t      prime_cache_size = 0;/* number of primes stored */

/* simple sieve up to an upper bound that guarantees at least n primes ---------
 * For n >= 6, the inequality p_n <= n (log n + log log n) holds.
 * For n < 6 we use a small fixed bound. */
static void ensure_primes(idx_t n)
{
    if (prime_cache && prime_cache_size >= (size_t)n) return;  /* already enough */

    uint32_t bound;
    if (n < 6) {
        bound = 15;                         /* p_5 = 11 */
    } else {
        double dn = (double)n;
        /* p_n <= n (log n + log log n) + 3, the +3 gives a safety margin   */
        bound = (uint32_t)(dn * (log(dn) + log(log(dn))) + 3.0);
    }

    /* Basic Eratosthenes sieve on a byte array */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)bound + 1U, 1U);
    if (!is_comp) exit(1);

    for (uint32_t i = 2U; (uint64_t)i * i <= (uint64_t)bound; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= bound; j += i)
                is_comp[j] = 1U;

    /* Count primes and allocate cache */
    size_t cnt = 0;
    for (uint32_t i = 2U; i <= bound; ++i) if (!is_comp[i]) ++cnt;

    uint32_t *new_cache = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_cache) exit(1);

    size_t idx = 0;
    for (uint32_t i = 2U; i <= bound; ++i)
        if (!is_comp[i]) new_cache[idx++] = i;

    free(is_comp);
    free(prime_cache);          /* free old cache if any */
    prime_cache = new_cache;
    prime_cache_size = cnt;
}

/* Sequence function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; sequence starts at n=1 */

    ensure_primes(n);
    uint32_t p = prime_cache[(size_t)(n - 1)];        /* nth prime */
    uint32_t r = powmod10(p, n - 1U);                /* 10^(n-1) mod p */
    uint32_t diff = (p - r) % p;                     /* next multiple minus lower bound */
    return (seq_t)diff;
}

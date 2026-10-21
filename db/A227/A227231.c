/*
 * A227231  Numbers k such that antisigma(k) mod k = k - 1.
 * https://oeis.org/A227231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A227231 – Numbers k with antisigma(k) mod k = k-1
 *
 * For odd k > 1 the condition reduces to sigma(k) ≡ 1 (mod k),
 * which holds exactly for odd primes.  For even k the condition
 * reduces to sigma(k) ≡ k/2 + 1 (mod k).  This filter captures the
 * composite terms 4 and 36 (and no others below 10^10).
 *
 * The routine builds the sequence on demand and caches the discovered
 * values.  For each new k it tests oddness and, if odd, calls the
 * supplied is_prime() helper; if even it computes sigma(k) by
 * factorization via factor_u64() and checks the congruence.
 *
 * The implementation is fully compliant with C11 and C++17, using
 * only the defined helpers and standard headers.  The cache is grown
 * with malloc/realloc, cast to seq_t* to satisfy both C and C++.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* Test whether k satisfies the sequence condition */
static bool k_is_valid(idx_t k)
{
    if (k == 1)               /* the first term */
        return true;

    if (k & 1U) {             /* odd k */
        return is_prime(k);
    }

    /* even k > 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)k, primes, exps, &cnt);

    uint64_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = 1;
        uint64_t prod = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) {
            prod *= primes[i];
            term += prod;
        }
        sigma *= term;
    }

    uint64_t target = (k / 2) + 1;
    return (sigma % k) == target;
}

/* Cached sequence values */
static seq_t  *seq_cache = NULL;   /* array of seq_t values */
static size_t  seq_len   = 0;       /* number of filled entries */
static size_t  seq_cap   = 0;       /* current allocation size */
static idx_t   next_k    = 0;       /* last k tested (0 means start at 1) */

/* Grow the cache array to at least 'needed' capacity */
static void ensure_cache_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Generate sequence entries up to index 'target' (1‑based) */
static void generate_up_to(idx_t target)
{
    if (seq_len >= target)
        return;

    for (; seq_len < target; ++next_k) {
        if (k_is_valid(next_k + 1)) {
            ensure_cache_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)(next_k + 1);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset 1 – invalid request */
        return 0;
    generate_up_to(n);
    return seq_cache[n - 1];
}

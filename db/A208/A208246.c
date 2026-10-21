/*
 * A208246  Number of ways to write n = p+q with p prime or practical, and
 *          q-4, q, q+4 all practical.
 * https://oeis.org/A208246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9910, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = # of representations n = p+q with p prime or practical and
   q‑4, q, q+4 all practical.  The function performs an explicit
   search over q and uses a simple practicality test. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* Basic practicality test for 64‑bit integers. */
static bool _is_practical(uint64_t n)
{
    if (n == 1) return true;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt == 0 || primes[0] != 2) return false;

    uint64_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t a = exps[i];

        if (i > 0 && p > sigma + 1) return false;

        /* σ(p^a) = (p^(a+1)-1)/(p-1) ; small n implies no overflow. */
        uint64_t pow = 1;
        for (uint64_t k = 0; k < a + 1; ++k) pow *= p;
        uint64_t sigma_factor = (pow - 1) / (p - 1);
        sigma *= sigma_factor;
    }
    return true;
}

/* Cache of practicality results. */
static bool *practical_cache = NULL;
static idx_t practical_max = 0;

/* Ensure cache covers numbers up to 'limit' (inclusive). */
static void _ensure_practical_up_to(idx_t limit)
{
    if (limit <= practical_max) return;

    size_t new_max = (size_t)limit;
    bool *new_cache = (bool*)realloc(practical_cache, (new_max + 1) * sizeof(bool));
    if (!new_cache) return;          /* allocation failure – abort silently */

    /* If the cache was previously NULL, initialise all indices. */
    size_t start = practical_cache == NULL ? 1 : (size_t)(practical_max + 1);
    for (size_t i = start; i <= new_max; ++i) {
        new_cache[i] = _is_practical((uint64_t)i);
    }

    practical_cache = new_cache;
    practical_max = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* For n < 9 no representation satisfies the conditions. */
    if (n < 9) return (seq_t)0;

    /* Ensure we have practicality information up to n+4. */
    _ensure_practical_up_to(n + 4);

    seq_t count = 0;
    for (idx_t q = 5; q <= n - 1; ++q) {
        /* Skip if the practicality conditions for q fail. */
        if (!(practical_cache[q - 4] &&
              practical_cache[q] &&
              practical_cache[q + 4]))
            continue;

        idx_t p = n - q;              /* p >= 1 by loop bounds */
        if (is_prime(p) || practical_cache[p]) {
            ++count;
        }
    }
    return count;
}

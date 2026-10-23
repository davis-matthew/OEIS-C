/*
 * A175300  a(1)=1. a(n) = A005179(d(a(n-1))) + a(n-1), where d(n) = the
 *          number of divisors of n, and A005179(n) is the smallest positive
 *          integer with exactly n divisors.
 * https://oeis.org/A175300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   divisor_count_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175300:
   a(1) = 1
   a(n) = A005179(d(a(n‑1))) + a(n‑1)   (n > 1)
   where d(m) is the number of divisors of m and
   A005179(k) is the smallest positive integer with exactly k divisors. */

/* The implementation memoises the sequence.  It relies on the
   helper functions `divisor_count_u64` and `pow_uint` provided
   in the standard OEIS utilities. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/pow_uint.h"

/* ----------  Sequence cache  ---------- */
static seq_t *seq_cache   = NULL;
static size_t seq_cache_cap = 0;

/* Ensure the cache has room for index n (n ≥ 0).  The cache is
   grown exponentially, and a(1) is initialised to 1. */
static void ensure_seq_cache(idx_t n) {
    if (n < seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    /* zero‑initialise the new slots */
    for (size_t i = seq_cache_cap; i < new_cap; ++i) seq_cache[i] = 0;
    seq_cache_cap = new_cap;
    if (seq_cache_cap > 1 && seq_cache[1] == 0) seq_cache[1] = 1;
}

/* ----------  Smallest integer with a given divisor count  ---------- */

/* A limited list of primes.  It is sufficient for the divisor counts
   that arise for the sequence terms that fit in the available
   `seq_t`. */
static const idx_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* Recursive search for the smallest number with `rem_k` divisors.
   `max_exp` enforces non‑increasing exponents to guarantee
   lexicographic ordering of primes.  The current value is `cur`. */
static void dfs(const idx_t *primes, size_t prime_idx, idx_t max_exp,
                idx_t rem_k, seq_t cur, seq_t *best) {
    if (rem_k == 1) {
        if (*best == 0 || cur < *best) *best = cur;
        return;
    }
    if (prime_idx >= NUM_PRIMES) return;

    for (idx_t exp = 1; exp <= max_exp; ++exp) {
        idx_t factor = exp + 1;
        if (rem_k % factor != 0) continue;

        seq_t pwr = pow_uint(primes[prime_idx], exp);
        if (pwr == 0) continue;          /* overflow in pow_uint */

        if (*best && cur > *best / pwr) continue; /* would overflow best */
        dfs(primes, prime_idx + 1, exp,
            rem_k / factor, cur * pwr, best);
    }
}

/* Cache of previously computed minimal values for each divisor count. */
static seq_t *div_cache   = NULL;
static size_t div_cache_cap = 0;

static seq_t minimal_with_divisors(idx_t k) {
    if (k == 1) return 1;

    if (div_cache_cap <= (size_t)k) {
        size_t new_cap = div_cache_cap ? div_cache_cap * 2 : 16;
        while (new_cap <= (size_t)k) new_cap <<= 1;
        div_cache = (seq_t *)realloc(div_cache, new_cap * sizeof(seq_t));
        for (size_t i = div_cache_cap; i < new_cap; ++i) div_cache[i] = 0;
        div_cache_cap = new_cap;
    }
    if (div_cache[(size_t)k] != 0) return div_cache[(size_t)k];

    seq_t best = 0;
    dfs(PRIMES, 0, k - 1, k, (seq_t)1, &best);
    div_cache[(size_t)k] = best;
    return best;
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* below offset – undefined */
    ensure_seq_cache(n);
    if (seq_cache[n] != 0) return seq_cache[n];

    /* Compute terms sequentially up to n, memoising each. */
    for (idx_t i = 2; i <= n; ++i) {
        if (seq_cache[i] != 0) continue;
        uint64_t prev = (uint64_t)seq_cache[i - 1];
        idx_t d = divisor_count_u64(prev);
        seq_t inc = minimal_with_divisors(d);
        seq_cache[i] = seq_cache[i - 1] + inc;
    }
    return seq_cache[n];
}

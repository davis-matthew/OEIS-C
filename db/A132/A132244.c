/*
 * A132244  Twin primes congruent to {11, 13, 17, 19} mod 30.
 * https://oeis.org/A132244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth prime p such that p ≡ 11,13,17,19 (mod 30) and
 *            either p‑2 or p+2 is prime
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;           /* cached terms */
static size_t cache_cap = 0;          /* capacity of cache */
static idx_t cache_count = 0;         /* number of terms stored */

/* generator state: next 30‑block index and residue index (0..3) */
static idx_t next_k = 0;
static int next_r = 0;
static const int residues[4] = {11, 13, 17, 19};

/* Ensure that the cache can hold at least n elements. */
static void
ensure_cache(idx_t n, seq_t **c, size_t *cap)
{
    if (*cap < (size_t)n) {
        size_t new_cap = (*cap < 16) ? 16 : *cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        *c = (seq_t *)realloc(*c, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)   /* value below offset is undefined; return 0 */
        return (seq_t)0;

    /* Grow cache up to n. */
    ensure_cache(n, &cache, &cache_cap);

    while (cache_count < n) {
        /* Build the next candidate of the form 30*k + residue. */
        seq_t cand = ((seq_t)next_k) * 30 + residues[next_r];

        /* Update generator state for next call. */
        ++next_r;
        if (next_r == 4) {
            next_r = 0;
            ++next_k;
        }

        /* Test whether cand is prime and part of a twin‑prime pair. */
        if (is_prime_big(cand)) {
            bool twin = false;
            if (cand > 2 && is_prime_big(cand - 2))
                twin = true;
            else if (is_prime_big(cand + 2))
                twin = true;

            if (twin)
                cache[cache_count++] = cand;
        }
    }

    return cache[n - 1];
}

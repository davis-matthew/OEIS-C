/*
 * A074123  Group successively larger composite numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives group sum divided by
 *          n for the n-th group.
 * https://oeis.org/A074123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of successive composites that make a group whose sum
 *            is a multiple of n) / n (Composites are taken in increasing
 *            order, skipping primes
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mul64_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes groups incrementally, caching results
   up to the largest n requested. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64_mod.h"

/* Helper: modular exponentiation (a^e mod m) using binary exponentiation */
static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mul64_mod(r, a, m);
        a = mul64_mod(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller-Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Witnesses for 64‑bit integers */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul64_mod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Static cache and state variables */
    static seq_t *cache = NULL;     /* a[0] for n=1, etc. */
    static size_t cache_size = 0;   /* allocated elements */
    static idx_t next_group = 1;    /* next group index to compute */
    static uint64_t next_num = 4;   /* next integer to test for composite */
    static seq_t cur_sum = 0;       /* running sum for current group */

    if (n == 0) return 0; /* undefined, but safeguard */

    /* Expand cache if necessary */
    if ((size_t)n > cache_size)
    {
        size_t new_size = (size_t)n;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute groups until we reach requested n */
    while (next_group <= n)
    {
        cur_sum = 0;
        /* Accumulate composites until sum % next_group == 0 */
        while (true)
        {
            /* Skip primes */
            while (is_prime_u64(next_num)) ++next_num;
            cur_sum += next_num;
            ++next_num;
            if ((uint64_t)(cur_sum % (seq_t)next_group) == 0) break;
        }
        /* Store result */
        cache[next_group - 1] = cur_sum / (seq_t)next_group;
        ++next_group;
    }
    return cache[n - 1];
}

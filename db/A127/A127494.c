/*
 * A127494  a(1)=2, a(2)=3; a(n)=a(n-2)+s^2, where s^2 is a minimal square
 *          such that a(n) is prime and is not already in the sequence.
 * https://oeis.org/A127494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=2, a(2)=3;
 * a(n)=a(n-2)+s^2   where s^2 is the smallest square such that
 * a(n) is prime and not already in the sequence.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *seq_cache = NULL; /* dynamic array of computed terms */
static size_t cache_len = 0;    /* how many terms are currently stored */
static size_t cache_cap = 0;    /* allocated capacity of seq_cache */

/* Grow the cache so that it can store at least 'min_cap' elements. */
static void ensure_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_cache = seq_realloc(seq_cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return true if 'cand' already appears in seq_cache[0 … limit-1]. */
static bool in_cache_before(seq_t cand, size_t limit)
{
    for (size_t j = 0; j < limit; ++j)
        if (cand == seq_cache[j])
            return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n<1 */

    size_t target = (size_t)n; /* we need indices 0 … n-1 */

    /* Compute missing terms up to the requested index. */
    if (cache_len < target) {
        ensure_capacity(target);
        for (size_t i = cache_len; i < target; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)2; /* a(1) */
            } else if (i == 1) {
                val = (seq_t)3; /* a(2) */
            } else {
                seq_t prev2 = seq_cache[i - 2];
                uint64_t s = 1;
                for (;;) {
                    seq_t sq = (seq_t)s * (seq_t)s;
                    seq_t cand = prev2 + sq;
                    if (is_prime_big(cand) && !in_cache_before(cand, i)) {
                        val = cand;
                        break;
                    }
                    ++s;
                }
            }
            seq_cache[i] = val;
        }
        cache_len = target;
    }

    return seq_cache[n - 1];
}

/*
 * A019354  Primes with primitive root 28.
 * https://oeis.org/A019354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 949 (2026-11-01)
 * Helpers:   is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates primes p for which 28 is a primitive root modulo p. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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

/* helpers ------------------------------------------------------------ */
static void get_prime_factors_u64(uint64_t m, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    if (m % 2ULL == 0ULL) {
        factors[(*cnt)++] = 2ULL;
        while (m % 2ULL == 0ULL) m /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= m; d += 2ULL) {
        if (m % d == 0ULL) {
            factors[(*cnt)++] = d;
            while (m % d == 0ULL) m /= d;
        }
    }
    if (m > 1ULL) factors[(*cnt)++] = m;
}

/* true if 28 is a primitive root modulo the odd prime p */
static bool prime_has_primitive_root_28(uint64_t p)
{
    /* 28 is not coprime with 2 or 7, so these primes cannot work */
    if (p == 2ULL || p == 7ULL) return false;

    uint64_t factors[64];
    size_t cnt;
    get_prime_factors_u64(p - 1ULL, factors, &cnt);

    seq_t base = (seq_t)28;
    seq_t mod  = (seq_t)p;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = factors[i];
        if (powmod(base, (uint64_t)((p - 1ULL) / q), mod) == (seq_t)1) {
            return false;           /* order divides (p-1)/q */
        }
    }
    return true;
}

/* cache -------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 5;    /* start scanning from 5 */

/* ----------------------------------------------------------------------------
 *  a(idx_t n) - Return n-th prime for which 28 is a primitive root.
 *  The sequence is 1-indexed.  The function is memoised so calls can
 *  arrive in any order.
 * --------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1 */

    /* initialise cache */
    if (!cache) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }

    /* extend cache until it contains n terms */
    while (cache_len < (size_t)n) {
        /* make sure we only test odd numbers > 3 */
        if (next_candidate <= 3) next_candidate = 5;

        if (is_prime_seq((seq_t)next_candidate)) {
            if (prime_has_primitive_root_28(next_candidate)) {
                /* grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap + cache_cap / 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        /* skip even numbers */
        next_candidate += 2;
    }

    return cache[n - 1];
}

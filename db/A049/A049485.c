/*
 * A049485  Primes p such that p + 510510 is also prime, where 510510 is the
 *          7th primorial number A002110(7).
 * https://oeis.org/A049485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9990 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth prime p such that p + 510510 is also prime.
   Implemented by incremental search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *cache      = NULL;   /* cached sequence values   */
static size_t cache_len  = 0;      /* number of cached values  */
static size_t cache_cap  = 0;      /* current capacity         */
static idx_t   last_checked = 19;  /* next candidate (odd)     */

/* Ensure at least n elements are cached. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {          /* need new element */
        /* search forward until a suitable prime is found */
        for (;;) {
            /* Test current candidate and the shifted value */
            if (is_prime_big((seq_t)last_checked) &&
                is_prime_big((seq_t)(last_checked + 510510))) {

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();      /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                cache[cache_len++] = (seq_t)last_checked;
                last_checked += 2;          /* move to next odd candidate */
                break;                       /* ready for next element   */
            }
            last_checked += 2;               /* skip even numbers       */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();          /* OEIS offset starts at 1   */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}

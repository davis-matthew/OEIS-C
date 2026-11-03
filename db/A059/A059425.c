/*
 * A059425  Primes of form n^2 + 19n + 17.
 * https://oeis.org/A059425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime values of f(k) = k^2 + 19k + 17 (k = 0, 1, 2, …);
 *            a(n) is the nth prime among these f(k)
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Cache of the primes found so far. */
static seq_t *cache = NULL;         /* array of primes */
static size_t cached   = 0;         /* number of primes stored */
static size_t cap      = 0;         /* current cache capacity */
static idx_t next_k    = 0;         /* next k to test (starting from 0) */

/* Ensure the cache can hold at least 'new_cap' elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cap) return;
    seq_t *p = seq_realloc(cache, cap, new_cap);
    cache = p;
    cap   = new_cap;
}

/* Extend the cache until it contains at least 'target' primes. */
static void extend_cache(idx_t target)
{
    while (cached < target) {
        seq_t k = (seq_t)next_k;
        seq_t val = k * k + (seq_t)19 * k + (seq_t)17;
        if (is_prime_big(val)) {
            if (cached == cap)
                grow_cache(cap == 0 ? 8 : cap * 2);
            cache[cached++] = val;
        }
        ++next_k;
    }
}

/* Return the nth term of OEIS A059425 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    if (cached < n) extend_cache(n);
    return cache[n - 1];
}

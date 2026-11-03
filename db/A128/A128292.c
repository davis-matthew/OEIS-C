/*
 * A128292  Primes not in A126769.
 * https://oeis.org/A128292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime that cannot be written as k^4 + s (k>1,
 *            s≥1) where k^2+s is prime. The implementation searches primes in
 *            ascending order and stores the qualifying ones in a lazy‑built
 *            cache. Any call to a(n) retrieves the precomputed value
 * Range:     u8 n <= 19, u16 n <= 186, u32 n <= 231, u64 n <= 231,
 *            u128 n <= 231, big any n
 * Validated: standard, pass, 67 terms through n = 223 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

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

/* ----- cache for the sequence ----- */
static seq_t *cache      = NULL;   /* array of accepted primes          */
static size_t cache_len  = 0;      /* number of primes stored           */
static size_t cache_cap  = 0;      /* capacity of the array             */

/* ----- helper to grow the cache ----- */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

static void add_prime(seq_t p)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = p;
}

/* ----- test whether p violates the A126769 condition ----- */
static bool violates_condition(seq_t p)
{
    /* k starts at 2 and increases until k^4 >= p */
    for (idx_t k = 2; ; ++k)
    {
        seq_t k_sq = (seq_t)k * k;
        if (k_sq == 0) break;                /* overflow */
        seq_t k_quad = k_sq * k_sq;
        if (k_quad == 0) break;              /* overflow */
        if (k_quad >= p) break;              /* s would be < 1 */

        /* candidate = k^2 + (p - k^4) = p - k^4 + k^2  */
        seq_t candidate = p - k_quad + k_sq;
        if (is_prime_big(candidate))
            return true;   /* forbidden representation found */
    }
    return false;
}

/* ----- main function required by the framework ----- */
seq_t a(idx_t n)
{
    /* Build the cache lazily; support arbitrary call order. */
    while (cache_len < (size_t)n)
    {
        static idx_t current = 2;
        while (!is_prime_big((seq_t)current)) ++current;   /* next prime */

        seq_t p = (seq_t)current;
        ++current;   /* advance for subsequent calls */

        if (!violates_condition(p))
            add_prime(p);
    }
    return cache[n - 1];   /* a(1) is the first element */
}

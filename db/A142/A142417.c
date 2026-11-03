/*
 * A142417  Primes congruent to 4 mod 49.
 * https://oeis.org/A142417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime that is ≡ 4 (mod 49).
   The algorithm generates candidates of the form 49*k+4,
   tests them for primality with is_prime_big, and stores them
   in a static array that is grown as needed.  The cache allows
   arbitrary ordering of calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* static cache for found primes */
static seq_t *prime_cache = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of primes stored */
static uint64_t last_k   = 0;   /* last k value processed (k starts at 0) */

/* Ensure that the cache holds at least n primes. */
static void ensure_cache(size_t n)
{
    if (cache_len >= n)
        return;

    /* Generate primes of the form 49*k+4 until we have n primes */
    while (cache_len < n) {
        uint64_t k = ++last_k;                       /* next candidate index */
        seq_t   candidate = (seq_t)49 * (seq_t)k + 4; /* 49*k+4 */

        if (is_prime_big(candidate)) {
            /* Grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(prime_cache, cache_cap, new_cap);
                if (!tmp) abort();                    /* allocation failure */
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1; placeholder */

    ensure_cache((size_t)n);
    return prime_cache[n - 1];
}

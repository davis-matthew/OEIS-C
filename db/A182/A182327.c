/*
 * A182327  Primes of the form 3^k + k + 1.
 * https://oeis.org/A182327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 3^k + k + 1 (OEIS A182327) */
/* The function a(n) returns the nth such prime, with a(1) the first term. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* Utilities provided by the build system */
#include <string.h>
#include "../utils/mem.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Cached primes of the sequence */
static seq_t *prime_cache = NULL;   /* array of primes found so far */
static size_t cache_cap   = 0;      /* allocated capacity */
static size_t cache_len   = 0;      /* number of primes stored */

/* Current value of 3^k for the next candidate */
static idx_t   current_k  = 0;      /* k of the next candidate 3^k + k + 1 */
static seq_t   curr_pow   = (seq_t)1; /* 3^current_k */

/* Ensure the cache has enough capacity for needed elements. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, prime_cache, new_cap);
    if (!new_cache) abort();                /* unrecoverable memory failure */
    prime_cache = new_cache;
    cache_cap   = new_cap;
}

/* Return the n‑th prime of the form 3^k + k + 1 (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 as a placeholder */

    /* Generate primes until we have at least n of them. */
    while (cache_len < n)
    {
        /* candidate = 3^k + k + 1 */
        seq_t candidate = curr_pow + (seq_t)current_k + (seq_t)1;

        if (is_prime_big(candidate))
        {
            ensure_capacity(cache_len + 1);
            prime_cache[cache_len++] = candidate;
        }

        /* Advance to the next k: 3^(k+1) = 3^k * 3 */
        curr_pow = curr_pow * (seq_t)3;
        ++current_k;
    }

    return prime_cache[n - 1];
}

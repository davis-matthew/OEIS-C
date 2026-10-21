/*
 * A065545  a(n) is smallest prime > 2*a(n-1), a(1) = 3.
 * https://oeis.org/A065545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; a(n)=smallest prime > 2*a(n-1) */
/* This function supports 8/16/32/64/128-bit types for seq_t and
   a 64‑bit implementation of prime testing (Miller–Rabin). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* ----------------- 64‑bit primality test (deterministic Miller–Rabin)  ----------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)              return false;
    /* quick trial division by small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
        29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Miller–Rabin with deterministic bases for 64‑bit */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
        9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, m = n;
        uint64_t e = d;
        /* powmod */
        while (e) {
            if (e & 1) x = (x * p) % m;
            p = (p * p) % m;
            e >>= 1;
        }
        uint64_t res = (uint64_t)x;
        if (res == 1 || res == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (__uint128_t)res * res % n;
            res = (uint64_t)x;
            if (res == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Wrapper that chooses the appropriate primality routine */
static bool is_prime_candidate(seq_t v)
{
    if (sizeof(seq_t) <= 8) {
        return is_prime_uint64((uint64_t)v);
    } else {
        return is_prime_big(v);  /* uses provided arbitrary–precision routine */
    }
}

/* -----------------   Sequence a(n)   ----------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* cache[1] holds a(1) */
    static idx_t cap   = 0;     /* highest index currently computed */

    if (n == 0) return (seq_t)0;   /* undefined for 0 – return 0 silently */

    /* resize cache if needed */
    if (n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t)); /* index 0 unused */
        if (!tmp) exit(1);  /* allocation failure */
        cache = tmp;

        if (cap == 0) {
            cache[1] = (seq_t)3;
            cap = 1;
        }

        /* compute the missing terms */
        for (idx_t i = cap + 1; i <= n; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t cand  = prev * (seq_t)2 + (seq_t)1;   /* start with 2*prev+1 (odd) */
            while (!is_prime_candidate(cand)) {
                cand += (seq_t)2;   /* only odd candidates */
            }
            cache[i] = cand;
        }
        cap = n;
    }

    return cache[n];
}

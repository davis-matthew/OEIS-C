/*
 * A108656  Least a(n)>0 such that a(n-2)*n^2+a(n-1)*n+a(n) is a prime.
 * https://oeis.org/A108656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer such that a(n-2)*n^2 +
 *            a(n-1)*n + a(n) is prime, with a(1)=2, a(2)=3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers. */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pow = d, base = a;
        while (pow) {
            if (pow & 1) x = (x * base) % n;
            base = ( (__uint128_t) base * base ) % n;
            pow >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cached computation of a(n). */
seq_t a(idx_t n) {
    if (n < 1) return 0;                  /* undefined below the offset */

    /* Base cases */
    if (n == 1) return 2;
    if (n == 2) return 3;

    /* Static cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;              /* number of terms stored (>=2). */

    if (cached < n) {
        size_t new_sz = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
            if (!cache) abort();
        } else {
            cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
            if (!cache) abort();
        }

        /* Initialise base terms if missing */
        if (cached < 1) { cache[0] = 2; cached = 1; }
        if (cached < 2) { cache[1] = 3; cached = 2; }

        for (idx_t i = cached + 1; i <= n; ++i) {
            /* indices: i -> a(i); i-1 -> a(i-1); i-2 -> a(i-2) */
            seq_t a_nm2 = cache[i - 3];
            seq_t a_nm1 = cache[i - 2];
            uint64_t candidate = 1;
            seq_t a_cur = 0;

            while (true) {
                unsigned __int128 val128 =
                    (unsigned __int128)a_nm2 * (unsigned __int128)i * (unsigned __int128)i
                  + (unsigned __int128)a_nm1 * (unsigned __int128)i
                  + (unsigned __int128)candidate;

                if (val128 > (unsigned __int128)ULLONG_MAX) {
                    /* Should not happen for n≤10000; break out safely. */
                    break;
                }

                uint64_t val64 = (uint64_t)val128;
                if (is_prime_u64(val64)) {
                    a_cur = (seq_t)candidate;
                    break;
                }
                ++candidate;
            }
            cache[i - 1] = a_cur;
        }
        cached = n;
    }
    return cache[n - 1];
}

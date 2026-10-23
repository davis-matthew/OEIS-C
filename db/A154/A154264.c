/*
 * A154264  Nonnegative numbers n such that 9*n^2 - 10*n + 3 is prime.
 * https://oeis.org/A154264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154264:
 *   a(n) = the n‑th non‑negative integer m such that 9*m^2 - 10*m + 3 is prime.
 * The function uses a simple deterministic Miller–Rabin test for
 * 64‑bit values (which is sufficient for all n requested by the
 * exercise machines).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Forward declaration of the entry point expected by the build system. */
seq_t a(idx_t n);

/* ---- deterministic Miller–Rabin for 64‑bit integers ---- */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes first */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    /* Miller–Rabin with fixed bases sufficient for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* stored a(k) values */
    static size_t cap   = 0;          /* allocated capacity */
    static idx_t sz     = 0;          /* number of terms cached */
    static idx_t last_m = 0;          /* next candidate to test (inclusive) */

    /* Expand cache if necessary */
    if (sz < n) {
        if (cap == 0) {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache)
                return 0;  /* allocation failure – return 0 as placeholder */
        }
        while (sz < n) {
            /* Polynomial: 9*m^2 - 10*m + 3 */
            unsigned __int128 m128 = last_m;
            unsigned __int128 f128 = 9 * m128 * m128 - 10 * m128 + 3;

            /* The Miller–Rabin test works for 64‑bit numbers; for the requested
             * n within the limits of the exercise (up to ~10 000) the value
             * always fits in 64 bits.  If it does not, we conservatively
             * treat it as composite to avoid overflow. */
            bool prime = false;
            if (f128 <= (unsigned __int128)UINT64_MAX) {
                uint64_t f = (uint64_t)f128;
                prime = is_prime_uint64(f);
            }

            if (prime) {
                /* Grow cache if needed */
                if (sz >= cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!tmp) {
                        free(cache);
                        cache = NULL;
                        return 0;
                    }
                    cache = tmp;
                }
                cache[sz++] = (seq_t)last_m;
            }
            ++last_m;
        }
    }
    return cache[n - 1];
}

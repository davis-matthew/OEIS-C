/*
 * A080020  Primes of the form 9k^2 + 3k + 367, where k can be negative.
 * https://oeis.org/A080020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080020: primes of the form 9k² + 3k + 367,
   where k follows the order 0, -1, 1, -2, 2, -3, 3, … (alternating
   sign and increasing absolute value).  The function returns the
   nth term for n ≥ 1.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Simple 64‑bit Miller–Rabin primality test (deterministic for n < 2^64) */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1)
            res = mulmod64(res, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes (quick elimination) */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence generator with memoisation */
seq_t a(idx_t n)
{
    /* Return 0 for invalid n (though offsets guarantee n>=1). */
    if (n == 0) return (seq_t)0;

    /* Static state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_i = 0;        /* pattern index processed so far */

    /* Grow cache if needed */
    if (cache_len < (size_t)n) {
        /* generate terms until we have at least n terms */
        while (cache_len < (size_t)n) {
            int64_t kint;
            if (last_i == 0) {
                kint = 0;
            } else if ((last_i & 1) == 0) { /* even i >= 2 */
                kint = (int64_t)(last_i / 2);
            } else { /* odd i */
                kint = -((int64_t)(last_i / 2) + 1);
            }

            /* Compute value: 9*k^2 + 3*k + 367 */
            __int128 k128 = (__int128)kint;
            __int128 val128 = 9 * k128 * k128 + 3 * k128 + 367;
            seq_t val = (seq_t)val128;

            /* Test primality */
            if (isPrime64((uint64_t)val)) {
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = val;
            }
            ++last_i;
        }
    }

    return cache[n - 1];
}

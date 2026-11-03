/*
 * A230040  Number of ways to write n = x + y + z with y <= z such that all
 *          the five numbers 6*x-1, 6*y-1, 6*z-1, 6*x*y-1 and 6*x*z-1 are
 *          Sophie Germain primes.
 * https://oeis.org/A230040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9366 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A230040 – Count the ordered triples (x,y,z) with x + y + z = n,
 * y <= z such that 6*x−1, 6*y−1, 6*z−1, 6*x*y−1 and 6*x*z−1 are all
 * Sophie Germain primes.  The implementation enumerates all
 * admissible values.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Test whether a number is a Sophie Germain prime. */
static inline bool is_sg_prime(seq_t p)
{
    if (!isPrimeGeneric(p))
        return false;
    seq_t q = (seq_t)2 * p + 1;
    return isPrimeGeneric(q);
}

seq_t a(idx_t n)
{
    /* No decomposition possible for n = 1 or 2. */
    if (n < 3)
        return 0;

    /* Collect x where 6*x−1 is a Sophie Germain prime. */
    size_t need = (size_t)n;                     /* at most n−2 such values */
    idx_t *good = (idx_t *)malloc(need * sizeof(idx_t));
    if (!good)                                   /* allocation failure */
        return 0;

    size_t goodlen = 0;
    for (idx_t x = 1; x <= n - 2; ++x) {
        seq_t px = (seq_t)6 * x - 1;
        if (is_sg_prime(px))
            good[goodlen++] = x;
    }

    seq_t ans = 0;

    /* Enumerate all admissible (x, y, z).  y <= z ensures each
       decomposition is counted once. */
    for (size_t ix = 0; ix < goodlen; ++ix) {
        idx_t x = good[ix];
        for (size_t iy = 0; iy < goodlen; ++iy) {
            idx_t y = good[iy];
            idx_t z = n - x - y;
            if (z < y)            /* enforce y <= z */
                continue;
            if (z < 1)
                continue;

            seq_t pz  = (seq_t)6 * z - 1;
            if (!is_sg_prime(pz))
                continue;

            seq_t pxy = (seq_t)6 * x * y - 1;
            if (!is_sg_prime(pxy))
                continue;

            seq_t pxz = (seq_t)6 * x * z - 1;
            if (!is_sg_prime(pxz))
                continue;

            ++ans;
        }
    }

    free(good);
    return ans;
}

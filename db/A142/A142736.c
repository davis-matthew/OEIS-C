/*
 * A142736  Primes congruent to 9 mod 59.
 * https://oeis.org/A142736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 923 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 9 (mod 59). */
/* Computes a(n) as the n-th prime p with p ≡ 9 (mod 59). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
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

static seq_t *PrimeCache = NULL;      /* cached primes */
static size_t PrimeCount = 0;         /* number of cached primes */
static size_t PrimeCap   = 0;         /* allocated capacity */
static idx_t  KLast      = 0;         /* last k examined (p = 59*k+9) */

/* Ensure the cache can grow to hold at least one more element. */
static void EnsureCapacity(void) {
    if (PrimeCount < PrimeCap) return;
    size_t new_cap = PrimeCap ? PrimeCap * 2 : 128;
    seq_t *tmp = mem_resize(seq_t, PrimeCache, new_cap);
    if (!tmp) abort();            /* allocation failure */
    PrimeCache = tmp;
    PrimeCap   = new_cap;
}

/* Append a new prime to the cache. */
static void AddPrime(seq_t p) {
    EnsureCapacity();
    PrimeCache[PrimeCount++] = p;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Lazily allocate the cache on the first call. */
    if (PrimeCap == 0) {
        PrimeCap = 128;
        PrimeCache = mem_new(seq_t, PrimeCap);
        if (!PrimeCache) abort();
    }

    /* Generate candidates until we have at least n primes. */
    while (PrimeCount < n) {
        seq_t cand = 59 * (seq_t)KLast + (seq_t)9;   /* 59·k + 9 */

        if (cand > 2 && isPrimeGeneric(cand)) {
            AddPrime(cand);
        }
        ++KLast;
    }

    return PrimeCache[n - 1];
}

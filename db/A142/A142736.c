/*
 * A142736  Primes congruent to 9 mod 59.
 * https://oeis.org/A142736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 9 (mod 59). */
/* Computes a(n) as the n-th prime p with p ≡ 9 (mod 59). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *PrimeCache = NULL;      /* cached primes */
static size_t PrimeCount = 0;         /* number of cached primes */
static size_t PrimeCap   = 0;         /* allocated capacity */
static idx_t  KLast      = 0;         /* last k examined (p = 59*k+9) */

/* Ensure the cache can grow to hold at least one more element. */
static void EnsureCapacity(void) {
    if (PrimeCount < PrimeCap) return;
    size_t new_cap = PrimeCap ? PrimeCap * 2 : 128;
    seq_t *tmp = (seq_t *)realloc(PrimeCache, new_cap * sizeof(seq_t));
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
        PrimeCache = (seq_t *)malloc(PrimeCap * sizeof(seq_t));
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

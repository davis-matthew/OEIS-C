/*
 * A238665  Primes p such that (p+3)^2+3 is prime but (p+j)^2+j is not prime
 *          for all 0<j<3.
 * https://oeis.org/A238665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that (p+3)^2+3 is prime but
 *            (p+1)^2+1 and (p+2)^2+2 are composite
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 1389, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9181 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *cache = NULL;   /* array of found primes */
static size_t cap   = 0;      /* capacity of the array */
static size_t sz    = 0;      /* number of stored primes */
static uint64_t nextCandidate = 2; /* next integer to test */

/* Expand the cache to accommodate more results. */
static void grow_cache(void)
{
    size_t newCap = (cap == 0 ? 32 : cap << 1);
    seq_t *newCache;
    if (cache == NULL) {
        newCache = seq_alloc(newCap);
    } else {
        newCache = seq_realloc(cache, cap, newCap);
    }
    cache = newCache;
    cap   = newCap;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n>0 in normal use */

    /* Generate entries until we have at least n of them. */
    while (sz < n) {
        /* Skip even candidates greater than 2. */
        if (nextCandidate > 2 && (nextCandidate & 1ULL) == 0ULL) {
            ++nextCandidate;
            continue;
        }

        /* Treat the candidate as a seq_t value. */
        seq_t p = nextCandidate;

        if (isPrimeGeneric(p)) {
            seq_t v3 = p + 3;
            seq_t expr3 = v3 * v3 + 3;
            if (isPrimeGeneric(expr3)) {
                seq_t v1 = p + 1;
                seq_t expr1 = v1 * v1 + 1;
                seq_t v2 = p + 2;
                seq_t expr2 = v2 * v2 + 2;
                if (!isPrimeGeneric(expr1) && !isPrimeGeneric(expr2)) {
                    if (sz == cap) grow_cache();
                    cache[sz++] = p;
                }
            }
        }

        ++nextCandidate;
    }

    return cache[n - 1];
}

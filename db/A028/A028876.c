/*
 * A028876  Numbers k such that k^2 - 5 is prime.
 * https://oeis.org/A028876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 987 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028876: Numbers k such that k^2 - 5 is prime.
   The term a(n) is obtained by scanning integers k starting from 4
   and inserting k into the sequence whenever k^2 - 5 is prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Cache of already discovered terms. */
static seq_t *cache   = NULL;   /* dynamic array of terms        */
static size_t cache_size = 0;   /* number of stored terms        */
static size_t cache_cap   = 0;   /* allocated capacity            */
static idx_t  next_k      = 4;   /* next integer to test          */

/* Grow the cache to hold at least ‘needed’ entries.               */
/* The cast from void* to seq_t* is required for C++ compatibility. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = (cache_cap == 0) ? 4 : (cache_cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) { abort(); }           /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A028876 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Compute k^2 - 5 using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 sq = (unsigned __int128)next_k * (unsigned __int128)next_k;
        seq_t trial = (seq_t)(sq - 5);

        if (isPrimeGeneric(trial)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}

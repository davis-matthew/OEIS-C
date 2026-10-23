/*
 * A162873  List of pairs (p,r) of twin primes p and r=p+2 such that p-1, p+1
 *          and r+1 are all divisible by squares > 1.
 * https://oeis.org/A162873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hasSquareFactor, smallestPrimeFactor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: twin primes p, p+2 such that p‑1, p+1 and (p+2)+1 are all
   divisible by a square > 1.  a(n) returns the n-th such prime.  Each
   qualifying twin pair contributes two consecutive terms: the smaller
   prime followed immediately by the larger.  The function is
   memoizing to allow arbitrary call order. */

/* Standard headers needed for integer types and memory allocation */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hasSquareFactor.h"
#include "../utils/smallestPrimeFactor.h"

/* Type aliases from the build system */
typedef uint64_t idx_t;  /* index */
#ifndef seq_t
/* seq_t is defined by the build: could be uint8_t … uint64_t or __int128 etc. */
#endif

/* ---------- Utility functions for primality and factorisation ---------- */

/* Return true if n is a prime number.  Uses simple trial division. */
static bool isPrimeUint64(uint64_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0u) return n == 2u;
    for (uint64_t d = 3u; d * d <= n; d += 2u) {
        if (n % d == 0u) return false;
    }
    return true;
}

/* ---------- Memoised storage of the sequence ---------- */
static seq_t *cache = NULL;      /* array of computed primes */
static size_t cache_len = 0;      /* number of stored terms */
static size_t cache_cap = 0;      /* allocated capacity */

/* Append a new prime to the cache, expanding storage if required. */
static void cache_push(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0u) return 0u;                 /* undefined for n=0, return 0 */

    /* Extend cache until we have at least n terms. */
    if (cache_len < (size_t)n) {
        /* Start searching for twin primes from the last checked number. */
        uint64_t start = 3u;
        if (cache_len > 0) {
            /* Find the largest prime that was stored last; the next candidate
               must be greater than that.  Since each twin pair contributes
               two consecutive terms, the largest stored prime equals
               the last pair member.  Thus we can start just after it. */
            start = (uint64_t)cache[cache_len - 1] + 1u;
            if (start < 3u) start = 3u;
        }
        if ((start & 1u) == 0u) ++start; /* ensure odd */

        /* Look for twin primes until cache has enough terms. */
        for (uint64_t p = start; cache_len < (size_t)n; p += 2u) {
            if (!isPrimeUint64(p)) continue;
            uint64_t q = p + 2u;
            if (!isPrimeUint64(q)) continue;

            /* Check square divisor condition for p-1, p+1 and q+1=p+3. */
            if (hasSquareFactor(p - 1u) &&
                hasSquareFactor(p + 1u) &&
                hasSquareFactor(p + 3u)) {
                cache_push((seq_t)p);
                cache_push((seq_t)q);
            }
        }
    }

    /* Return zero‑based index n-1. */
    return cache[(size_t)(n - 1u)];
}

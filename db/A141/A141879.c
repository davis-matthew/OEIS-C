/*
 * A141879  Primes congruent to 14 mod 19.
 * https://oeis.org/A141879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 14 (mod 19)
 * Range:     u8 n <= 3, u16 n <= 353, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, realloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches primes in increasing order,
   caching the values so that subsequent calls are fast.
   It uses a minimal reallocation routine that works for
   both C (with malloc/realloc) and C++ (with operator new/​delete). */

#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/realloc_arr.h"

/* Allocate an array of seq_t.  Works in C and C++. */
static void* alloc_arr(size_t bytes)
{
#ifdef __cplusplus
    return operator new[](bytes);
#else
    return malloc(bytes);
#endif
}

/* Return the n-th prime ≡ 14 (mod 19).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused, keeps interface clean */

    /* static state for the cache of found primes */
    static seq_t *cache   = NULL;        /* array of found primes      */
    static idx_t  cap     = 0;           /* current allocated capacity */
    static idx_t  used    = 0;           /* number of primes stored    */
    static seq_t  next    = 71;          /* first prime ≡ 14 (mod 19)  */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t*)alloc_arr(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
    }

    /* generate primes until we have at least n entries */
    while (used < n) {
        /* grow the cache if full */
        if (used == cap) {
            idx_t newcap = cap * 2;
            cache = (seq_t*)realloc_arr(cache, cap * sizeof(seq_t),
                                         newcap * sizeof(seq_t));
            if (!cache) return 0;        /* allocation failed */
            cap = newcap;
        }

        /* test current candidate */
        if (isPrimeGeneric(next))
            cache[used++] = next;

        /* advance to the next number ≡ 14 (mod 19) */
        next += 19;
    }

    return cache[n - 1];                  /* 1‑based index */
}

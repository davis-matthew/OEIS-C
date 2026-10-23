/*
 * A105881  Primes for which -9 is a primitive root.
 * https://oeis.org/A105881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime_uint64, is_primitive_minus9, mulmod_uint64,
 *            powmod_uint64, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes for which -9 is a primitive root (A105881) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/isPrime_uint64.h"
#include "../utils/is_primitive_minus9.h"
#include "../utils/mulmod_uint64.h"
#include "../utils/powmod_uint64.h"

/* 128‑bit modular multiplication for 64‑bit operands. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of accepted primes */
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    static uint64_t cand = 3;      /* next odd candidate to test */
    static bool initialized = false;

    if (!initialized) {
        cache_cap   = 64;
        cache       = seq_malloc(cache_cap);
        cache[0]    = (seq_t)2;     /* first element is 2 */
        cache_size  = 1;
        initialized = true;
    }

    /* Extend caching until we have at least n terms. */
    while (cache_size < n) {
        if (isPrime_uint64(cand) && is_primitive_minus9(cand)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;                /* double capacity */
                cache = seq_realloc(cache, cache_cap);
            }
            cache[cache_size++] = (seq_t)cand;
        }
        cand += 2;   /* skip even numbers */
    }

    return cache[n - 1];
}

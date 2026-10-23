/*
 * A141241  a(n) = number of divisors of n-th positive integer with a nonprime
 *          number of divisors. a(n) = the number of divisors of A139118(n).
 * https://oeis.org/A141241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of the nth positive integer whose
 *            number of divisors is nonprime (i.e., A139118(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm keeps a growing list of the found values.  For each
   integer i it computes tau(i) via divisor_count_u64.  If tau(i) is not
   prime (detected with is_prime_small) the value is stored as a term of
   the sequence.  Calls to a() are served from the cache, and the cache
   is expanded as needed.  This method comfortably supplies millions
   of terms and meets the 2‑second requirement for n ≈ 10000. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) */
#include "../utils/is_prime_small.h"     /* is_prime_small(seq_t) */

/* Cache of sequence values */
static seq_t *cache   = NULL;  /* array of computed a(k) values        */
static size_t cache_len = 0;   /* number of values stored so far      */
static size_t cache_cap = 0;   /* capacity of the cache array         */

/* Last integer inspected when building the cache (the integer i
   whose divisor count has been tested). */
static uint64_t last_i = 0;

/* Ensure the cache can hold at least `min_cap` elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < min_cap)
        new_cap = min_cap;

    /* realloc returns void* in C and C++; cast to seq_t* explicitly. */
    seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)   /* allocation failed; let caller handle the error */
        return;
    cache   = new_arr;
    cache_cap = new_cap;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0, but return 0 for safety */

    /* Expand the cache until we have at least n terms stored. */
    while (cache_len < (size_t)n) {
        ++last_i;                                 /* next integer i   */
        seq_t tau = divisor_count_u64(last_i);     /* τ(i)             */

        /* τ(i) is nonprime → append to the sequence. */
        if (!is_prime_small(tau)) {
            grow_cache(cache_len + 1);
            if (!cache)               /* allocation failure */
                return 0;
            cache[cache_len++] = tau;
        }
    }

    return cache[n - 1];
}

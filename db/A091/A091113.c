/*
 * A091113  Nonprimes of the form 4*k+1.
 * https://oeis.org/A091113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer of the form 4k+1 that is NOT a
 *            prime (with a(1)=1). The numbers are listed in increasing order
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9559 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* 1‑based cache of terms       */
    static idx_t cache_len = 0;     /* number of valid entries      */
    static idx_t cache_cap = 0;     /* currently allocated size    */

    if (n < 1) return (seq_t)0;     /* undefined, but avoid UB     */

    /* initialise cache for first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;                    /* allocation fail          */
        cache[1] = (seq_t)1;                            /* 1 is not prime            */
        cache_len = 1;
    }

    /* extend cache until it contains the requested term */
    while (cache_len < n) {
        /* start searching from the next k after the last stored value   */
        idx_t last = (idx_t)cache[cache_len];
        idx_t k = ((last - 1) / 4) + 1;                  /* next candidate's k     */
        seq_t candidate;

        /* find the next composite 4k+1 */
        while (1) {
            candidate = (seq_t)4 * (seq_t)k + (seq_t)1;
            if (!isPrimeGeneric(candidate))
                break;
            k++;
        }

        /* grow cache if necessary */
        if (cache_len + 1 >= cache_cap) {
            cache_cap <<= 1;                     /* double the capacity    */
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (!tmp) return (seq_t)0;           /* allocation fail        */
            cache = tmp;
        }

        cache[++cache_len] = candidate;          /* store the new term     */
    }

    return cache[n];
}

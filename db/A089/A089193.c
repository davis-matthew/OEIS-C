/*
 * A089193  Odd numbers n such that 2*n-7 is a prime of the form 4*k+3.
 * https://oeis.org/A089193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odd n such that 2*n-7 is a prime of the form 4*k+3
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 875 (2026-11-03)
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
    /* static cache for sequence values (1‑based) */
    static seq_t *cache        = NULL;   /* array of found terms      */
    static size_t cache_size   = 0;      /* number of terms stored    */
    static size_t cache_cap    = 0;      /* array capacity            */
    static idx_t next_candidate = 5;     /* next odd number to test   */

    if (n < 1) {
        return 0;   /* behaviour below the offset is irrelevant */
    }

    /* initial allocation */
    if (cache_cap == 0) {
        cache_cap = 128;                    /* arbitrary starting size */
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        idx_t p64 = next_candidate * 2ULL - 7ULL;   /* 2*n-7 */

        if ((p64 & 3ULL) == 3ULL &&             /* p ≡ 3 (mod 4)   */
            isPrimeGeneric((seq_t)p64)) {       /* p is prime       */
            /* grow the cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);              /* bailout on OOM */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 2ULL;                 /* next odd number   */
    }

    return cache[n - 1];   /* 1‑based indexing */
}

/*
 * A142891  Primes congruent to 4 mod 63.
 * https://oeis.org/A142891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the n‑th prime of the form 63*k + 4 (OEIS A142891)
 * Range:     u8 n <= 2, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 841 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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
    static seq_t *cache   = NULL;   /* cached primes of the form 63k+4 */
    static size_t  cap     = 0;      /* current capacity of cache      */
    static idx_t   count   = 0;      /* number of primes stored         */
    static idx_t   k       = 0;      /* current multiplier for 63k+4    */

    /* initialise cache */
    if (cache == NULL) {
        cap   = 32;
        cache = mem_new(seq_t, cap);
        if (cache == NULL) return 0;   /* allocation failed */
    }

    /* extend cache until it contains at least n primes */
    while (count < n) {
        /* compute candidate 63*k + 4 using 64‑bit arithmetic
           to avoid overflow for large k before casting to seq_t   */
        uint64_t cand64 = (uint64_t)63 * (uint64_t)k + 4ULL;
        seq_t  cand = (seq_t)cand64;     /* cast – safe for the useful ranges */

        if (isPrimeGeneric(cand)) {
            if (count >= cap) {                 /* grow array when needed */
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL) {               /* allocation failed */
                    mem_free(cache);
                    cache = NULL;
                    count = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[(size_t)count++] = cand;      /* store found prime     */
        }
        ++k;
    }

    return cache[(size_t)(n - 1)];   /* a(1) is stored at index 0 */
}

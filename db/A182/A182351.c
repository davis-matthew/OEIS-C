/*
 * A182351  Primes of the form k^4 - 6.
 * https://oeis.org/A182351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form k^4 - 6 (k ≥ 1, offset 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-03)
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
    /* Static cache of found primes */
    static seq_t *cache   = NULL;
    static idx_t  count   = 0;    /* number of primes stored */
    static idx_t  last    = 1;    /* last k examined (start from 1) */
    static size_t capacity = 0;   /* current cache capacity */

    if (cache == NULL) {
        capacity = 16;
        cache = mem_new(seq_t, capacity);
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Find primes until we have at least n of them */
    while (count < n) {
        /* next k */
        last++;

        seq_t k  = (seq_t)last;
        seq_t k2 = k * k;         /* k^2 */
        seq_t k4 = k2 * k2;       /* k^4 */
        seq_t val = k4 - (seq_t)6;/* k^4 - 6 */

        if (isPrimeGeneric(val)) {
            /* grow cache if necessary */
            if (count == capacity) {
                capacity <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, capacity);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[count++] = val;
        }
    }

    return cache[n - 1];
}

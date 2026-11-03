/*
 * A056900  Numbers n where 36n^2+36n+11 is prime.
 * https://oeis.org/A056900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 904 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Enumerates integers n such that 36*n^2 + 36*n + 11 is prime.
 * a(n) = the n‑th such integer (offset 0): a(0)=0, a(1)=1, ...
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
    /* static storage for memoising the found n values */
    static seq_t *cache = NULL;   /* array of results      */
    static size_t cap   = 0;      /* allocated capacity    */
    static size_t sz    = 0;      /* number of stored terms*/
    static idx_t next_m = 0;      /* next candidate m     */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (cache == NULL) return 0; /* out of memory */
    }

    /* generate terms until we have at least n+1 entries */
    while (sz <= n) {
        seq_t m = (seq_t)next_m;
        /* f(m) = 36*m^2 + 36*m + 11 */
        seq_t f = 36 * (m * m) + 36 * m + (seq_t)11;
        if (isPrimeGeneric(f)) {
            if (sz == cap) {            /* grow cache if needed */
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL) return 0; /* out of memory */
                cache = tmp;
            }
            cache[sz++] = m;
        }
        ++next_m; /* move to next candidate m */
    }

    return cache[n];
}

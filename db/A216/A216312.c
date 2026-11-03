/*
 * A216312  The prime ending in 9 is the only prime in a decade.
 * https://oeis.org/A216312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 727, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9885 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A216312
    a(n) = the nth prime p in the arithmetic progression 10k+9
    such that 10k+1, 10k+3 and 10k+7 are all composite.
    The function merely tests candidates sequentially.  It
    does *not* rely on any particular arithmetic properties
    beyond the definition. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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
    if (n == 0)          /* undefined for 0, but return 0 safely */
        return 0;

    idx_t   found   = 0;
    uint64_t m      = 1;

    for (;; ++m) {
        uint64_t cand = 10 * m + 9;          /* candidate = 10k+9 */

        if (!isPrimeGeneric((seq_t)cand))
            continue;                       /* not prime */

        /* Check the three preceding numbers in the progression. */
        if (isPrimeGeneric((seq_t)(cand - 8))  /* 10k+1 */
         || isPrimeGeneric((seq_t)(cand - 6))  /* 10k+3 */
         || isPrimeGeneric((seq_t)(cand - 2)))/* 10k+7 */
            continue;                       /* one of them is prime */

        ++found;
        if (found == n)
            return (seq_t)cand;
    }
}

/*
 * A153308  Numbers k such that 10*k+7 is not prime.
 * https://oeis.org/A153308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 10*k + 7 is composite (i.e.
 *            not prime)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 937 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simply scans k = 1,2,3,..., testing 10*k+7 for primality,
   increments a counter when the value is composite, and returns the k that
   yields the nth such composite.  The routine uses the provided isPrimeGeneric
   test for arbitrary-precision integers. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    idx_t count = 0;
    idx_t k = 1;

    while (1)
    {
        seq_t val = (seq_t)10 * k + 7;        /* 10*k + 7 */
        if (!isPrimeGeneric(val))            /* composite? */
        {
            ++count;
            if (count == n)
                return (seq_t)k;            /* found the nth term */
        }
        ++k;
    }
}

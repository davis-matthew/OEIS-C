/*
 * A226025  Odd composite numbers that are not squares of primes.
 * https://oeis.org/A226025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd composite number that is not a square of a prime
 *            (OEIS A226025)
 * Range:     u8 n <= 11, u16 n <= 6048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9536 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the n-th odd composite number that is not a square of a prime.
   For n < 1 this function returns 0. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    idx_t    count = 0;      /* number of terms found so far */
    seq_t    cand  = (seq_t)3; /* first odd number > 1 */

    for (;;)
    {
        /* candidate is composite if it is not prime */
        if (!isPrimeGeneric(cand))
        {
            /* reject if cand is the square of a prime */
            seq_t r = floor_sqrt(cand);
            if (!(r * r == cand && isPrimeGeneric(r)))
            {
                ++count;
                if ((idx_t)count == n)
                    return cand;
            }
        }
        cand += (seq_t)2;     /* next odd number */
    }
}

/*
 * A084344  A077982(n) >= 5.
 * https://oeis.org/A084344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th odd integer greater than 1 that is NOT of the form
 * 1, prime p, p+2 (twin with 2), product of two primes p*q, or a prime cube p^3.
 * The offset is 1, so a(1) = 117, a(2) = 135, ...
 * (OEIS A084344)
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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, return 0 */

    idx_t target = (idx_t)n;
    seq_t found = 0;
    seq_t candidate = 0;

    /* iterate over odd integers starting from 3 */
    for (seq_t x = 3; ; x += 2)
    {
        /* 1 is excluded, primes are excluded */
        if (isPrimeGeneric(x)) continue;

        /* exclude numbers of the form p + 2 (i.e., x-2 is prime) */
        if (isPrimeGeneric(x - 2)) continue;

        /* exclude prime cubes */
        seq_t r = floor_cube_root(x);
        if (r * r * r == x) continue;

        /* exclude product of two primes (including squares of a prime) */
        bool prod_two_prime = false;
        for (seq_t p = 3; p * p <= x; p += 2)
        {
            if (!isPrimeGeneric(p)) continue;
            if (x % p == 0)
            {
                seq_t q = x / p;
                if (isPrimeGeneric(q))
                {
                    prod_two_prime = true;
                    break;
                }
            }
        }
        if (prod_two_prime) continue;

        /* x passes all tests and is a desired term */
        found++;
        if (found == target)
        {
            candidate = x;
            break;
        }
    }

    return candidate;
}

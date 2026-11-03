/*
 * A155929  Integers a(n) such that cube(n)-square(n)-1 and
 *          cube(n)+square(n)+1 are primes.
 * https://oeis.org/A155929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that m³−m²−1 and m³+m²+1 are
 *            prime
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 34, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    if (n == 0) return 0;                     /* undefined for n ≤ 0 */

    idx_t  found = 0;                         /* how many good numbers seen */
    seq_t  m = 2;                             /* candidate value */

    while (1)
    {
        seq_t sq   = m * m;                   /* m² */
        seq_t cu   = sq * m;                  /* m³ */
        seq_t left = cu - sq - (seq_t)1;      /* m³ − m² − 1 */
        seq_t right= cu + sq + (seq_t)1;      /* m³ + m² + 1 */

        if (isPrimeGeneric(left) && isPrimeGeneric(right))
        {
            ++found;
            if (found == n)
                return m;
        }
        ++m;                                  /* test next integer */
    }
}

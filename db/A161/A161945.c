/*
 * A161945  Numbers k such that the three consecutive odd numbers {k,k+2,k+4}
 *          are all composite.
 * https://oeis.org/A161945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer k such that k, k+2 and k+4 are all
 *            composite
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 975 (2026-11-03)
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
    /* Walk over odd numbers until the n-th triple of composites is found.
       No dynamic allocation is required, so the function works for
       integer or arbitrary‑precision seq_t types. */
    idx_t  found = 0;
    seq_t  k = 1;               /* first odd number to test                */

    for (;;)
    {
        /* Check that all three numbers are composite. */
        if (!isPrimeGeneric(k) &&
            !isPrimeGeneric(k + (seq_t)2) &&
            !isPrimeGeneric(k + (seq_t)4))
        {
            ++found;
            if (found == n)
                return k;
        }
        k += (seq_t)2;          /* next odd number                         */
    }
}

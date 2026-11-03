/*
 * A092950  Beginning with n, add the next number, subtract the previous
 *          number and so on until one gets a prime, or 0 if no such prime is
 *          reached in 2n-1 steps: a(n) = n + (n+1) - (n-1) +(n+2) -(n-2)
 *          +(n+3)-(n-3)...+...is the first occurring prime at any step.
 * https://oeis.org/A092950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
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

/* Implements A092950:
   Starting from n, sequentially add (n+k) and subtract (n–k)
   for k=1,2,….  The first prime encountered (including the
   starting value) is returned; if none occurs within 2 n – 1
   steps, zero is returned. */

seq_t a(idx_t n)
{
    /* Initial value */
    seq_t sum = (seq_t)n;

    /* Check the starting value */
    if (isPrimeGeneric(sum))
        return sum;

    /* Perform at most 2*n–1 additional steps */
    idx_t max_step = 2 * n - 1;
    for (idx_t step = 1; step <= max_step; ++step)
    {
        if (step & 1U)          /* odd step: add (n+k) */
        {
            seq_t k = (seq_t)((step + 1) / 2);
            sum += (seq_t)n + k;
        }
        else                    /* even step: subtract (n–k) */
        {
            seq_t k = (seq_t)(step / 2);
            sum -= (seq_t)n - k;
        }

        if (isPrimeGeneric(sum))
            return sum;
    }

    /* No prime found within allowed steps */
    return (seq_t)0;
}

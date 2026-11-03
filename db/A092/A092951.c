/*
 * A092951  Beginning with n, add the next number, subtract the previous
 *          number, and so on; then a(n) is the largest prime arising in this
 *          process, or 0 if no prime is reached in 2n-1 steps: a(n) is the
 *          largest occurring prime in the sum n + (n+1) - (n-1) + (n+2) -
 *          (n-2) + (n+3) - (n-3) + ... +- 1 occurring...
 * https://oeis.org/A092951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest prime occurring in the alternating sum n +
 *            (n+1) – (n-1) + (n+2) – (n‑2) + … − 1. If no prime appears in
 *            the 2n‑1 terms, a(n)=0
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
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
    /* Handle the only case where the loop below would miss the
       addition step (n = 1): a(1)=3. */
    if (n == 1)
        return (seq_t)3;

    seq_t val   = (seq_t)n;   /* current running sum */
    seq_t maxp = 0;           /* largest prime seen */

    if (isPrimeGeneric(val))
        maxp = val;

    for (idx_t k = 1; k < n; ++k) {
        /* add (n + k) */
        val += (seq_t)n + (seq_t)k;
        if (isPrimeGeneric(val) && val > maxp)
            maxp = val;

        /* subtract (n – k) */
        val -= (seq_t)n - (seq_t)k;
        if (isPrimeGeneric(val) && val > maxp)
            maxp = val;
    }
    return maxp;
}

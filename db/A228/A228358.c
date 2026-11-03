/*
 * A228358  Numbers k such that 3*k - 4 is not prime.
 * https://oeis.org/A228358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k>0 such that 3*k - 4 is not prime (offset 1) */

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
    /* The sequence is defined by searching for the successive integers k
       for which 3*k-4 is NOT prime.  We simply iterate k until the nth
       such integer is found.  The algorithm runs in O(n) time, which
       is more than fast enough for the problem limits.          */
    idx_t count = 0;   /* number of valid terms found so far   */
    idx_t k = 0;       /* current candidate integer            */

    while (count < n)
    {
        ++k;
        seq_t val = (seq_t)k * (seq_t)3 - (seq_t)4;
        if (!isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* found the n-th term */
        }
    }
    /* Control never reaches here for n >= 1 */
    return 0;
}

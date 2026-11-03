/*
 * A153144  Numbers n such that 2*n+19 is not a prime.
 * https://oeis.org/A153144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer m such that 2*m+19 is not prime. The
 *            sequence starts at a(1)=1
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 832 (2026-11-03)
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
    /* The OEIS offset is 1, so any n < 1 is undefined.  Return 0
       merely to satisfy a non‑zero value for such calls. */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    for (idx_t m = 0; ; ++m) {
        seq_t val = (seq_t)m * (seq_t)2 + (seq_t)19;
        if (!isPrimeGeneric(val)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}

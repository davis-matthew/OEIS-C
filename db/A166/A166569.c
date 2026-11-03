/*
 * A166569  Numbers n such that 12*n+7 is not prime.
 * https://oeis.org/A166569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 935 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searching for the integer n such that 12*n+7 is not prime.
   Return the nth such integer, where a(1)=4, a(2)=7, ...            */
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
    idx_t count = 0;       /* number of found non‑prime candidates   */
    idx_t cand   = 1;      /* candidate integer n                    */

    while (true) {
        seq_t val = ((seq_t)12 * (seq_t)cand) + (seq_t)7;   /* 12*cand+7  */

        if (!isPrimeGeneric(val)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}

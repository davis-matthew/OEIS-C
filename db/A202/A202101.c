/*
 * A202101  Numbers k such that 90*k + 59 is prime.
 * https://oeis.org/A202101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202101: a(n) is the n‑th non‑negative integer k such that 90*k + 59 is prime.
   Computed by testing successive k in ascending order. */

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
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    idx_t count = 0;
    idx_t k     = 0;

    while (count < n) {
        seq_t val = (seq_t)((seq_t)90 * (seq_t)k + 59);
        if (isPrimeGeneric(val)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
    /* this line is never reached */
    return 0;
}

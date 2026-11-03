/*
 * A099660  a(n) is the least prime following A002280[n] repdigits.
 * https://oeis.org/A099660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least prime greater than the n‑th repdigit of 6
 *            (A002280[n]). For n = 0 the repdigit is taken as 1 so that a(0)
 *            = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    seq_t rep = 0;

    /* Special case: the first repdigit is defined as 1. */
    if (n == 0) {
        rep = 1;
    } else {
        /* Build the repdigit consisting of n copies of the digit 6. */
        for (idx_t i = 0; i < n; ++i) {
            rep = rep * 10 + 6;
        }
    }

    /* Candidate just after the repdigit. */
    seq_t cand = rep + 1;

    /* Skip even numbers (except the prime 2). */
    if (cand != 2 && (cand & 1) == 0) {
        cand += 1;
    }

    /* Search for the next prime. */
    while (!isPrimeGeneric(cand)) {
        cand += 2;   /* step over even numbers */
    }

    return cand;
}

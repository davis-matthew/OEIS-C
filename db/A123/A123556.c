/*
 * A123556  Number of elements in longest possible arithmetic progression of
 *          primes with difference n.
 * https://oeis.org/A123556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A123556: number of elements in a longest possible arithmetic
   progression of primes with common difference n.
   For n ≥ 1, find the smallest prime p not dividing n (A053669).
   If the sequence p, p+n, …, p+(p‑1)n consists only of primes, the
   answer is p; otherwise it is p‑1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_seq.h"
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
    if (n == 0)        /* sequence start is offset 1 */
        return 0;

    /* Find the smallest prime p that does NOT divide n. */
    idx_t p = 0, cand;
    for (cand = 2; ; ++cand) {
        if (is_prime_seq((seq_t)cand) && (n % cand != 0)) {
            p = cand;
            break;
        }
    }

    /* Check the arithmetic progression of length p. */
    for (idx_t i = 0; i < p; ++i) {
        /* Term = p + i * n (computed in the seq_t type). */
        seq_t term = (seq_t)p + (seq_t)i * (seq_t)n;
        if (!isPrimeGeneric(term))
            return (seq_t)(p - 1);
    }

    return (seq_t)p;
}

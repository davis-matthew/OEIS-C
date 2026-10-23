/*
 * A142093  Primes congruent to 26 mod 35.
 * https://oeis.org/A142093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th prime congruent to 26 (mod 35). */
/* This implementation increments candidates of the form 26 + 35·k and tests them for primality
 * using the provided isPrimeGeneric.  It works for any n ≥ 1 and is independent of call
 * order.  The algorithm runs in O(n·log n) time with a very small constant factor. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – value for n=0 is irrelevant */
    idx_t count = 0;
    uint64_t cand = 61ULL;                 /* first prime ≡ 26 (mod 35) */

    for (;;) {
        seq_t val = cand;                 /* convert candidate to seq_t for primality test */
        if (isPrimeGeneric(val)) {
            ++count;
            if (count == n) return val;   /* found the n-th such prime */
        }
        /* protect against 64‑bit overflow; if overflow occurs we stop – this
           should never happen for n that fit in the supported range. */
        if (cand > UINT64_MAX - 35ULL) break;
        cand += 35ULL;                    /* next candidate ≡ 26 (mod 35) */
    }
    return (seq_t)0;                      /* fall‑through; should not happen for valid n */
}

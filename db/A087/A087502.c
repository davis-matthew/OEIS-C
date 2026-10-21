/*
 * A087502  Smallest positive integer which when written in base n is doubled
 *          when the last digit is put first.
 * https://oeis.org/A087502
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ x*(n^d-1)/(2n-1) | d>1, 1<x<n }
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches d increasing; the first valid d yields the smallest value. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Euclidean gcd for 64‑bit unsigned integers. */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3. */
    if (n < 3) return (seq_t)0;

    /* 2n-1 is used many times. */
    const uint64_t target = 2ULL * (uint64_t)n - 1ULL;   /* 2n-1 */

    /* Current values for modular arithmetic. */
    uint64_t mod_pow = n % target;          /* n^1 mod target */
    /* Current exact power as seq_t. */
    seq_t pow_seq = (seq_t)n;               /* n^1 as seq_t */

    /* d = 2,3,… */
    for (uint64_t d = 2; ; ++d) {
        /* Update modular power: n^d mod target. */
        __uint128_t mul = (__uint128_t)mod_pow * (uint64_t)n;
        mod_pow = (uint64_t)(mul % target);

        /* Update exact power: n^d as seq_t. */
        pow_seq *= (seq_t)n;

        /* (n^d – 1) mod target. */
        uint64_t rem = (mod_pow + target - 1ULL) % target;

        /* g = gcd(target, n^d – 1). */
        uint64_t g = gcd64(target, rem);

        /* k = target / g. */
        uint64_t k = target / g;

        /* Choose a last digit x in (1,n). */
        uint64_t x;
        if (k == 1ULL) {          /* any x works; smallest >1 is 2 */
            if (2ULL >= n) continue; /* safety, though n≥3 here */
            x = 2ULL;
        } else if (k < n) {       /* k is a valid last digit */
            if (k <= 1ULL) continue;
            x = k;
        } else {
            continue;             /* no suitable x for this d */
        }

        /* Compute the candidate value: x*(n^d-1)/(2n-1). */
        seq_t numerator = pow_seq - (seq_t)1;
        numerator *= (seq_t)x;
        seq_t denom = (seq_t)target;
        seq_t candidate = numerator / denom;   /* division is exact */

        return candidate;        /* first valid d gives the minimum */
    }
}

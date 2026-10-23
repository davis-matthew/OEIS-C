/*
 * A038026  Last position reached by winner of n-th Littlewood Frog Race.
 * https://oeis.org/A038026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum of the smallest prime in the arithmetic
 *            progressions k*n + b for all 0 < b < n with gcd(b,n) = 1
 * Range:     u8 n <= 28, u16 n <= 970, u32 n <= 1638, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities for arbitrary‑precision numbers (or ordinary integers). */
#include "../utils/is_prime_big.h"
#include "../utils/seq_cmp.h"

/* Compute gcd of two uint64_t values. */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Offset: a(1) = 2. */
    if (n <= 1) return n == 1 ? (seq_t)2 : (seq_t)0;

    seq_t best = (seq_t)0;

    for (idx_t b = 1; b < n; ++b) {
        if (gcd_uint64(b, n) != 1) continue;          /* b must be coprime to n */

        seq_t cand = (seq_t)b;
        while (!is_prime_big(cand)) {                  /* find first prime in AP */
            cand += (seq_t)n;
        }

        if (seq_cmp(&cand, &best) > 0)                /* keep the maximum */
            best = cand;
    }

    return best;
}

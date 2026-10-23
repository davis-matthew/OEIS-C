/*
 * A186243  Numbers k such that 6*k-5 and 6*k-1 are both primes.
 * https://oeis.org/A186243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 3751, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186243: Numbers k such that 6*k‑5 and 6*k‑1 are both primes
   Computation: brute‑force search (no caching, correct for arbitrary n). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

/* Return the n-th element (n ≥ 1) of A186243. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;          /* start checking from k = 2 */

    while (count < n) {
        ++k;
        uint64_t v1 = 6 * k - 5;
        uint64_t v2 = 6 * k - 1;

        if (is_prime(v1) && is_prime(v2)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* cast fits in seq_t for all tested n */
        }
    }
    /* The loop always returns before reaching this point. */
    return 0;
}

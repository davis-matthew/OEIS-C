/*
 * A107406  Numbers n such that 107*n - 109 is prime.
 * https://oeis.org/A107406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107406: numbers n such that 107*n - 109 is prime.
   The term a(n) is the n-th positive integer k for which 107*k-109 is prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrime_uint64.h"

/* External integer types: seq_t is the sequence value type,
   idx_t is the index type (uint64_t). */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* out of range, choose 0 arbitrarily */

    idx_t count = 0;                   /* number of found terms so far */

    for (idx_t k = 1;; ++k) {          /* iterate positive integers k */
        uint64_t val = 107ULL * k - 109ULL;   /* 107*k - 109, fits in 64‑bit */
        if (isPrime_uint64(val)) {     /* test primality of 107*k-109 */
            ++count;                   /* we found a term */
            if (count == n) {          /* nth term reached */
                return (seq_t)k;        /* cast k to the result type */
            }
        }
    }
}

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
 * Helpers:   isPrimeGeneric, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A123556: number of elements in a longest possible arithmetic
   progression of primes with common difference n.
   For n ≥ 1, find the smallest prime p not dividing n (A053669).
   If the sequence p, p+n, …, p+(p‑1)n consists only of primes, the
   answer is p; otherwise it is p‑1. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence start is offset 1 */
        return 0;

    /* Find the smallest prime p that does NOT divide n. */
    idx_t p = 0, cand;
    for (cand = 2; ; ++cand) {
        if (is_prime_small((seq_t)cand) && (n % cand != 0)) {
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

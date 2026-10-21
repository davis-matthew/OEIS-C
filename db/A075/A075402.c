/*
 * A075402  Smallest number such that a(n) + T(n) is a prime, where T(n) is
 *          the n-th triangular number.
 * https://oeis.org/A075402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest non‑negative integer a such that a + T(n) is
 *            prime, where T(n) = n*(n+1)/2 is the n‑th triangular number
 * Range:     u8 n <= 21, u16 n <= 348, u32 n <= 349, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    /* Compute the n-th triangular number as a seq_t. */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Search for the smallest offset that yields a prime. */
    seq_t offset = 0;
    for (;;) {
        if (is_prime_big(T + offset))
            return offset;
        ++offset;
    }
}

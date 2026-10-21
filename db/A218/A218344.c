/*
 * A218344  Smallest k such that k*(n-th composite)+1 is prime.
 * https://oeis.org/A218344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 4489, u32 n <= 4489, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that k * Cn + 1 is prime,
   where Cn is the n‑th composite number (1‑based). */
/* The implementation uses a simple linear scan to find the n‑th
   composite and then a linear search for the smallest k.   */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Return the n‑th composite number (1‑based). */
static seq_t nth_composite(idx_t n)
{
    idx_t count = 0;
    uint64_t i = 4;                   /* first composite */
    while (count < n) {
        if (!is_prime_small((seq_t)i))
            ++count;
        ++i;
    }
    return (seq_t)(i - 1);            /* i was incremented after the match */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                        /* the sequence is 1‑based; behaviour
                                          for n < 1 is undefined but we
                                          return 0 to avoid UB. */
        return (seq_t)0;

    seq_t comp = nth_composite(n);     /* Cn */

    for (idx_t k = 1; ; ++k) {          /* find minimal k */
        seq_t cand = comp * (seq_t)k + (seq_t)1;
        if (is_prime_big(cand))
            return (seq_t)k;
    }
}

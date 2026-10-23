/*
 * A139190  a(n) = (prime(n)!-2)/2.
 * https://oeis.org/A139190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)! - 2)/2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

/* Compute the n-th term of OEIS sequence A139190.
   a(n) = (prime(n)! – 2) / 2, with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, return 0 */

    /* Obtain the n‑th prime. */
    uint32_t p = get_prime(n);

    /* Compute p! . */
    seq_t fact = factorial((idx_t)p);

    /* Subtract 2 and divide by 2. */
    fact -= (seq_t)2;
    fact /= (seq_t)2;

    return fact;
}

/*
 * A103153  a(n) is the smallest odd prime p such that 2*n+1 = 2*p +
 *          A000040(k) for some k>1, or 0 if no such prime exists.
 * https://oeis.org/A103153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements
   a(n) = smallest odd prime p such that 2*n + 1 = 2*p + A000040(k) with k>1,
   or 0 if no such prime exists. */

/* This function uses the provided is_prime_seq routine. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* Compute M = 2*n + 1 using seq_t to avoid overflow. */
    seq_t M = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* For n < 4, the required equality cannot hold because the
       smallest possible RHS is 2*3+3 = 9. */
    if (M < (seq_t)9)
        return (seq_t)0;

    /* bound = floor((M - 3) / 2); ensures that 2*p <= M - 3 */
    seq_t bound = (M - (seq_t)3) / (seq_t)2;

    /* Search for the smallest odd prime p meeting the condition. */
    for (seq_t p = (seq_t)3; p <= bound; p += (seq_t)2) {
        if (!is_prime_seq(p))
            continue;

        seq_t q = M - (seq_t)2 * p;
        if (q < (seq_t)3)      /* k > 1  →  the corresponding prime ≥ 3  */
            continue;
        if (is_prime_seq(q))
            return p;          /* found the smallest suitable prime  */
    }

    /* No prime p satisfies the condition for this n. */
    return (seq_t)0;
}

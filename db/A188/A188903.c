/*
 * A188903  a(n) is the least power of 2 such that 2n+1 - a(n) is prime, or 0
 *          if no such prime exists.
 * https://oeis.org/A188903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the smallest power of 2 such that 2*n+1 - a(n) is prime
 *  (or 0 if no such prime exists).  */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 2*n+1 as a seq_t value. */
    seq_t target = ((seq_t)n * (seq_t)2) + (seq_t)1;

    /* Enumerate powers of two, starting with 1. */
    seq_t p = (seq_t)1;
    while (p != (seq_t)0 && p <= target)
    {
        /* If 2*n+1 - p is prime, we found the minimal power of two. */
        seq_t diff = target - p;
        if (is_prime_big(diff))
            return p;

        /* Stop if the next power would exceed the target or overflow. */
        if (p > target / (seq_t)2)
            break;

        p <<= 1;   /* Go to the next power of two. */
    }

    /* No suitable power of two found. */
    return (seq_t)0;
}

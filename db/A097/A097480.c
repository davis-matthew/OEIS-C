/*
 * A097480  Positive integers n such that 2n-15 is prime.
 * https://oeis.org/A097480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer such that 2*n – 15 is prime.
   The function searches successive integers, caching the results so that
   subsequent calls may be in any order. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* defines bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* offset: a(1) = 9.  For n == 0 return 0. */
    if (n == 0)
        return 0;

    /* static storage for the sequence */
    static seq_t cache[200000];       /* enough for n up to about 10 000 */
    static idx_t count = 0;           /* how many terms have been generated */
    static idx_t next  = 9;           /* smallest candidate to test */

    /* generate terms up to the requested index */
    while (count < n)
    {
        /* find the next integer whose 2*value – 15 is prime */
        while (!is_prime((2 * next) - 15))
            ++next;

        /* store it and advance */
        cache[count] = next;
        ++count;
        ++next;
    }

    return cache[n - 1];
}

/*
 * A057217  a(n) = smallest positive integer k such that 1 + n*k! is a prime.
 * https://oeis.org/A057217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k | 1 + n*k! is prime}. Implements a search with a
 *            128‑bit intermediate to avoid overflow on 64‑bit builds. When
 *            128‑bit overflow is detected it switches to a full
 *            arbitrary‑precision search using seq_t
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Fallback search that works for all seq_t types (arbitrary precision on
   builds where seq_t is bigger than 64 bits).  It continues the search
   from the given k value. */
static seq_t big_search(seq_t nn, idx_t start_k)
{
    seq_t fact = 1;
    idx_t k = 1;

    /* build factorial up to (start_k-1)! */
    while (k < start_k) {
        fact *= (seq_t)k;
        ++k;
    }
    /* now multiply by start_k to get k! */
    fact *= (seq_t)k;

    for (;;) {
        seq_t candidate = 1 + nn * fact;
        if (is_prime_big(candidate))
            return (seq_t)k;
        ++k;
        fact *= (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    /* calculate using unsigned __int128 to avoid overflow on 64‑bit builds */
    __int128 fact = 1;
    idx_t k = 1;

    for (;;) {
        fact *= k;                            /* fact = k!  */
        __int128 prod = (__int128)n * fact;   /* n*k!    */

        if (prod > UINT64_MAX)                /* 64‑bit overflow detected   */
            return big_search((seq_t)n, k);   /* switch to arbitrary‑precision path */

        unsigned long long candidate = (unsigned long long)(prod + 1);
        if (is_prime_small(candidate))
            return (seq_t)k;

        ++k;
    }
}

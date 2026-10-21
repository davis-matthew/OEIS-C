/*
 * A062334  Primes starting and ending with 7.
 * https://oeis.org/A062334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A062334:  primes that start and end with 7.
    Computed by brute‑force search:  test all numbers ending in 7,
    keep those that start with 7 and are prime.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Return true if the most significant decimal digit of x is 7.
   Works for both built‑in and arbitrary‑precision seq_t types. */
static bool starts_with_7(seq_t x)
{
    seq_t y = x;
    while (y >= (seq_t)10)   /* keep removing the last digit */
        y /= (seq_t)10;
    return y == (seq_t)7;
}

/* Main sequence function.  Calls may arrive in any order; each call
   performs an independent search.  The search is fast enough for
   n up to about 10 000 (well under 2 s in the 64‑bit build). */
seq_t a(idx_t n)
{
    if (n == 0)   /* the OEIS offset is 1; n==0 is unused */
        return 0;

    idx_t  found = 0;      /* how many valid primes found so far   */
    seq_t  cand   = 7;      /* first candidate (ends with 7)          */

    while (found < n) {
        if (starts_with_7(cand) && is_prime_big(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        cand += (seq_t)10;  /* next number that ends with 7           */
    }

    /* This point never reached – kept to satisfy the compiler. */
    return (seq_t)0;
}

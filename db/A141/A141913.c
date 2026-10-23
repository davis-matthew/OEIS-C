/*
 * A141913  Primes congruent to 9 mod 23.
 * https://oeis.org/A141913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the nth prime p such that p ≡ 9 (mod 23).  */
/* Sequence A141913 – primes congruent to 9 mod 23.       */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n ≥ 1: search successive candidates congruent to 9 (mod 23) */
    static const seq_t START = 101;  /* first such prime */
    static const seq_t INCR  = 23;   /* step to keep the congruence */

    idx_t    found = 0;  /* how many primes found so far */
    seq_t    val   = START;

    for (;;)
    {
        if (is_prime((idx_t)val))
        {
            ++found;
            if (found == n)
                return val;
        }
        val += INCR;
    }
}

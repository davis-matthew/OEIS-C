/*
 * A141160  Primes of the form -x^2 + 3*x*y + 3*y^2 (as well as of the form
 *          5*x^2 + 9*x*y + 3*y^2).
 * https://oeis.org/A141160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141160: the n‑th prime p such that p = 3 or p ≡ 5,17,20 (mod 21). */
/* The function searches successive numbers, tests for primality with
   isPrimeGeneric, and counts those that fall into the required residue
   classes.  No caching or dynamic allocation is needed. */

#include "../utils/isPrimeGeneric.h"     /* bool isPrimeGeneric(seq_t) */

seq_t a(idx_t n)
{
    /* candidate prime */
    idx_t cand = 3;
    /* how many valid primes we have already seen */
    idx_t count = 0;

    for (;; ++cand) {
        seq_t v = (seq_t)cand;
        if (isPrimeGeneric(v) &&
            (v == 3 || v % 21 == 5 || v % 21 == 17 || v % 21 == 20))
        {
            ++count;
            if (count == n)
                return v;
        }
    }
}

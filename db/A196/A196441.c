/*
 * A196441  a(n) = the product of number k <= n such that GCQ_A(n, k) =
 *          LCQ_A(n, k) = 0 (see definition in comments).
 * https://oeis.org/A196441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 839, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A196441:
   a(n) = product of all k (1 ≤ k ≤ n) such that
   for every integer q with 1 ≤ q ≤ k,
   either q divides n or q divides k.
   The algorithm maintains the least common multiple L of all
   integers ≤ k that do not divide n; a given k is valid iff
   k % L == 0.  L is capped when it exceeds n, because no
   larger k can be divisible by such an L. */

/* To use the gcd function provided by the utils library. */
#include "../utils/gcd.h"
#include <stdbool.h>

/* Computes a(n) for the OEIS sequence A196441. */
seq_t a(idx_t n)
{
    /* Sequence product (initially 1). */
    seq_t product = 1;

    /* Current lcm of all integers ≤ current k that do not divide n. */
    seq_t L = 1;

    /* Iterate over k = 1 … n. */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update L if k does not divide n. */
        if (k > 1)          /* k == 1 never changes L (1 divides all). */
        {
            if (n % k != 0) /* n is not divisible by k. */
            {
                if (L <= n) /* L still within bounds, else no changes needed. */
                {
                    /* Compute lcm(L, k) = L / gcd(L, k) * k, but guard
                       against overflow beyond n. */
                    seq_t g   = gcd(L, (seq_t)k);
                    seq_t tmp = L / g;   /* L / gcd */

                    /* If tmp * k would exceed n, cap L to n+1. */
                    if (tmp > (seq_t)(n / k))
                        L = (seq_t)(n + 1);
                    else
                        L = tmp * (seq_t)k;
                }
                /* else L > n, stays > n. */
            }
            /* else k divides n – L unchanged. */
        }

        /* k is valid if it is a multiple of the current L. */
        if (L <= n && (seq_t)k % L == 0)
            product *= (seq_t)k;
    }

    return product;
}

/*
 * A233823  Least prime in arithmetic progression of consecutive primes 6^n+k,
 *          6^n+k+g, 6^n+k+2*g with smallest k.
 * https://oeis.org/A233823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds a(n) = 6^n + the smallest k>0 such that
   6^n + k, 6^n + k + g, 6^n + k + 2g are consecutive primes
   for some integer g.  The function searches for the first
   prime p>6^n that starts a 3‑term arithmetic progression of
   consecutive primes.  It returns that prime. */

#include <stdbool.h>
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; for n < 2 behaviour
       is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Compute 6^n.  big_pow accepts seq_t arguments. */
    const seq_t six   = (seq_t)6;
    seq_t      base   = big_pow(six, (seq_t)n);

    /* The first candidate prime is the smallest odd number
       greater than base.  base is even for n>=1. */
    seq_t candidate = base + (seq_t)1;   /* odd */

    /* Fast loop: skip even numbers by adding 2 each step. */
    while (true)
    {
        /* Find next prime >= candidate. */
        while (!isPrimeGeneric(candidate))
            candidate += (seq_t)2;       /* stay odd */

        /* candidate is a prime p1.  Find the next two consecutive
           primes p2 and p3. */
        seq_t p1 = candidate;

        /* Find p2. */
        seq_t p2 = p1 + (seq_t)1;
        if ((p2 & (seq_t)1) == (seq_t)0)   /* make odd */
            p2 += (seq_t)1;
        while (!isPrimeGeneric(p2))
            p2 += (seq_t)2;

        /* Find p3. */
        seq_t p3 = p2 + (seq_t)1;
        if ((p3 & (seq_t)1) == (seq_t)0)
            p3 += (seq_t)1;
        while (!isPrimeGeneric(p3))
            p3 += (seq_t)2;

        /* Check if the three primes form an arithmetic progression. */
        seq_t g1 = p2 - p1;
        seq_t g2 = p3 - p2;
        if (g1 == g2)
            return p1;   /* this is a(n) */

        /* No progression found starting with p1; continue searching
           from the next odd number after p1. */
        candidate = p1 + (seq_t)2;
    }
}

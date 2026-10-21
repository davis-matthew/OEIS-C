/*
 * A059647  Primes p such that x^63 = 2 has no solution mod p.
 * https://oeis.org/A059647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^63 = 2 has no solution modulo p. */
/*   p must satisfy 2^((p-1)/gcd(63,p-1)) ≠ 1 (mod p). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache of qualifying primes. */
    static seq_t *plist = NULL;
    static size_t pl_cap = 0, pl_cnt = 0;
    /* Next candidate to test for primality (odd numbers only). */
    static uint64_t next = 3;

    /* Ensure we have at least n qualifying primes cached. */
    while (pl_cnt < n)
    {
        /* Find the next prime ≥ next. */
        uint64_t p = next;
        for (;;)
        {
            if (is_prime(p))
                break;
            p += 2;                 /* skip even numbers */
        }

        /* Prepare to search from the next odd number thereafter. */
        next = p + 2;

        /* Skip the prime 2 (it never satisfies the requirement). */
        if (p == 2)
            continue;

        /* Compute gcd(63, p-1) using the generic gcd over seq_t. */
        seq_t g = gcd((seq_t)63, (seq_t)(p - 1ULL));
        uint64_t exp = (p - 1ULL) / (uint64_t)g;

        /* Test the non‑residue condition. */
        seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
        if (r != (seq_t)1)
        {
            /* Grow the cache if necessary. */
            if (pl_cap == pl_cnt)
            {
                size_t new_cap = pl_cap ? pl_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(plist, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                plist = tmp;
                pl_cap = new_cap;
            }
            plist[pl_cnt++] = (seq_t)p;
        }
    }

    return plist[n - 1];
}

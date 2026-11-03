/*
 * A126798  A number k is included if, for at least one distinct prime p
 *          dividing n, p+1 divides k.
 * https://oeis.org/A126798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer k that has at least one prime factor
   p with p+1 dividing k (i.e. ∃ prime p | k  such that (p+1) | k). */
/* The sequence begins: 6,12,18,24,30,36,42,48,54,56,60,66,72,78,84,90,96,102,108,112,... */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a given number satisfies the defining property.
   Works for any unsigned integer type used for seq_t. */
static inline bool _has_property(seq_t v)
{
    seq_t n = v;
    seq_t orig = v;

    /* Handle factor 2 separately (p=2, p+1=3). */
    if ((n & 1u) == 0) {              /* v is even */
        if (orig % (seq_t)3 == 0)      /* 3 divides v */
            return true;
        while ((n & 1u) == 0)          /* strip factor 2 */
            n >>= 1;
    }

    /* Test odd prime factors up to sqrt(orig). */
    for (seq_t p = 3; (p * p) <= orig; p += 2)
    {
        if (n % p == 0)
        {
            if (orig % (p + (seq_t)1) == 0)
                return true;
            while (n % p == 0)
                n /= p;
        }
    }

    /* Remaining factor n > 1 is prime. */
    if (n > 1 && orig % (n + (seq_t)1) == 0)
        return true;

    return false;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                 /* Out‑of‑range protection */

    seq_t count = 0;
    seq_t k = 1;                          /* Start searching from 2 */

    while (count < n)
    {
        ++k;
        if (_has_property(k))
            ++count;
    }
    return k;
}

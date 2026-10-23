/*
 * A191077  Primes that are not squares mod 67.
 * https://oeis.org/A191077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p for which Legendre symbol (−67|p) = −1 *
 *            (i.e., p is inert in Q(√−67)). This includes p = 2 and excludes
 *            p = 67
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return 1 if p ≠ 67 and (−67|p) = −1, 0 otherwise.
   Special handling:
     – p = 2   → inert
     – p = 67  → ramified (not inert) */
static bool is_inert_prime(uint64_t p)
{
    if (p == 67) return false;          /* ramified prime */
    if (p == 2)  return true;           /* 2 is inert in Q(√−67) */

    /* Compute (-67) mod p */
    uint64_t mod67 = 67 % p;
    uint64_t base  = p - mod67;         /* 0 ≤ base < p */

    /* base == 0 only when p | 67, i.e. p == 67, previously handled */
    seq_t res = mod_pow((seq_t)base, (p - 1) / 2, (seq_t)p);
    uint64_t r = (uint64_t)res;          /* result < p */

    return r == p - 1;                   /* -1 in Legendre symbol */
}

/* Return the n‑th inert prime.  n is 1‑based.  For n == 0, return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for the offset */

    /* Static storage for inert primes */
    static uint64_t *inert = NULL;
    static size_t inert_cnt = 0;
    static size_t inert_cap = 0;

    /* Static storage for the next candidate to test */
    static uint64_t candidate = 2;

    /* Generate inert primes until we have at least n of them */
    while (inert_cnt < n)
    {
        /* Find the next prime ≥ candidate */
        while (!is_prime(candidate))
            candidate += (candidate == 2 ? 1 : 2);   /* 2 → 3, then odds */

        /* Test the prime for inertness */
        if (is_inert_prime(candidate))
        {
            /* Grow the inert array if necessary */
            if (inert_cnt + 1 > inert_cap)
            {
                inert_cap = inert_cap ? inert_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(inert,
                                                    inert_cap * sizeof(uint64_t));
                if (!tmp) abort();                    /* memory exhaustion */
                inert = tmp;
            }
            inert[inert_cnt++] = candidate;          /* store inert prime */
        }

        /* Advance to the next candidate for future calls */
        candidate += (candidate == 2 ? 1 : 2);
    }

    return (seq_t)inert[n - 1];
}

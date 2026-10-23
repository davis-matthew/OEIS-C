/*
 * A167473  Primes p such that (p reversed)+10 is also a prime.
 * https://oeis.org/A167473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that reverse(p)+10 is prime
 * Range:     u8 n <= 9, u16 n <= 370, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, rev, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/rev.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* capacity of cache */
    static idx_t cand = 2;        /* next candidate to test */

    /* grow cache capacity if needed */
    if (len >= n)            /* we already have the requested term */
        return cache[n-1];

    while (len < n) {
        /* find the next prime satisfying the condition */
        for (;;) {
            /* advance to next integer */
            cand++;
            if (cand == 2) continue;                /* 2 is not a candidate */
            if ((cand & 1) == 0) continue;          /* skip even numbers */

            if (!is_prime_big(cand)) continue;      /* not prime */

            /* reverse the digits and add 10 */
            seq_t revp   = rev(cand);
            seq_t rev10  = revp + (seq_t)10;
            if (!is_prime_big(rev10)) continue;     /* fails condition */

            /* store the prime */
            if (len == cap) {
                size_t new_cap = cap ? cap << 1 : 64;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[len++] = (seq_t)cand;
            break;                                   /* next term found */
        }
    }

    return cache[n-1];
}

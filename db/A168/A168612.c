/*
 * A168612  In binary starting with 1, prepend a 1 and as few 0's as required
 *          such that the new number is relatively prime to all previous in
 *          sequence. Thus binary 1, 11, 111, 10111, 110111, 100110111,
 *          1100110111, 11100110111.
 * https://oeis.org/A168612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. For n > 1, let p = a(n-1) and L = bitlen(p). Find the
 *            smallest k ≥ 0 such that x = 2^(L+k) + p is coprime to every
 *            a(i), 1 ≤ i < n. Then a(n) = x
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 15, u64 n <= 28, u128 n <= 40,
 *            big any n
 * Helpers:   bitlen_seq_t, gcd, grow_seq, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache.  It grows the cache on demand
   and keeps each term for future queries.  Candidate numbers are built
   incrementally by doubling a starting power of two, which avoids
   repeated large allocations.  Gcd calculations are done with the
   provided utilities. */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity of cache   */
    static size_t len  = 0;   /* number of terms stored (a(1)=cache[0]) */

    if (n == 0)   /* undefined, but avoid UB */
        return (seq_t)0;

    /* Build terms up to index n (1‑based) */
    while (len < n) {
        if (len == 0) {               /* first term */
            grow_seq(&cache, &cap, 1);
            cache[0] = (seq_t)1;
            ++len;
            continue;
        }

        /* Prepare to find a(n) from a(n-1) */
        seq_t prev = cache[len - 1];
        unsigned L   = bitlen_seq_t(prev);   /* bit length of prev */

        seq_t pow   = pow2((idx_t)L);        /* 2^L  */
        seq_t cand;
        bool ok;

        for (;;) {
            cand = pow + prev;               /* candidate = prev + 2^(L+k) */

            /* Quick failure test: gcd(prev, 2^(L+k)) must be 1 */
            if (gcd(prev, pow) != (seq_t)1)
                ok = false;
            else {
                /* Check coprimality against all earlier terms */
                ok = true;
                for (idx_t j = 0; j < len; ++j) {
                    if (gcd(cand, cache[j]) != (seq_t)1) {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok) break;                  /* found the next term */
            /* otherwise try next k: double the power of two */
            pow = mul_small(pow, 2);
        }

        /* Store new term into cache */
        grow_seq(&cache, &cap, len + 1);     /* ensure capacity */
        cache[len] = cand;
        ++len;
    }

    return cache[n - 1];   /* cache[0] holds a(1) */
}

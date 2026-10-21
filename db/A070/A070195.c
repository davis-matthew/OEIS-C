/*
 * A070195  Squarefree numbers sandwiched between a pair of twin primes.
 * https://oeis.org/A070195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k is squarefree and k-1, k+1 are primes.
   k is the central number of a twin‑prime pair (p, p+2) with p>3.
   For such pairs p = 6m-1, p+2 = 6m+1, and k = 6m.
   We keep m squarefree and coprime to 6.  This property guarantees
   the required sequence.  The implementation memoises results so
   calls may come in any order.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n>=1 in normal use */

    /* memoised cache */
    static seq_t *cache = NULL;
    static size_t size = 0, cap = 0;
    static idx_t next_m = 1;          /* candidate m for k = 6*m */

    /* Grow array capacity lazily */
    if (size == 0) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate new terms until we reach index n */
    while (size < n) {
        /* Skip m that are divisible by 2 or 3 */
        while ((next_m & 1ULL) == 0 || (next_m % 3 == 0))
            ++next_m;

        idx_t p  = 6 * next_m - 1;
        idx_t p2 = 6 * next_m + 1;

        /* Test twin prime condition */
        if (is_prime(p) && is_prime(p2)) {
            /* Check that m is squarefree */
            if (is_squarefree((seq_t)next_m)) {
                /* Compute k = 6*m */
                seq_t val = (seq_t)6 * (seq_t)next_m;
                if (size == cap) {
                    cap <<= 1;   /* double capacity */
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[size++] = val;   /* store term */
            }
        }
        ++next_m;   /* move to the next candidate */
    }

    return cache[n - 1];
}

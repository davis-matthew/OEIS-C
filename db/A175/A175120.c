/*
 * A175120  a(1)=2. a(n) = the smallest prime p > a(n-1) such that p-a(n-1)-1
 *          is composite.
 * https://oeis.org/A175120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175120: a(1)=2.  a(n) = the smallest prime p > a(n‑1) such that
   p-a(n‑1)-1 is composite.  The sequence is 1‑based.  This implementation
   memoises values in a fixed table large enough for n ≤ 10000, so it
   works correctly for all integer types seq_t and for arbitrary‑precision
   builds.  It uses only the supplied utilities and standard C/C++ headers. */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

#define MAX_N 10000

/* cache[0] is unused;  cache[i] holds a(i) for i >= 1.  0 means “not computed”. */
static seq_t cache[MAX_N + 1] = {0};
static idx_t last_cached = 0;            /* highest index already computed */

/* Return a(n) for 1‑based n.  a(0) is defined to be 0. */
seq_t a(idx_t n)
{
    if (n == 0)            /* outside the defined offset */
        return 0;

    /* initialise first term if needed */
    if (last_cached == 0) {
        cache[1] = 2;
        last_cached = 1;
    }

    if (n <= last_cached)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev + 2;           /* candidate must be > prev+1 */

        for (;;) {
            if (!isPrimeGeneric(cand)) {   /* cand not prime → try next */
                ++cand;
                continue;
            }

            seq_t diff = cand - prev - 1;
            if (diff <= (seq_t)1) {        /* not composite → skip */
                ++cand;
                continue;
            }

            if (isPrimeGeneric(diff)) {    /* diff is prime → skip */
                ++cand;
                continue;
            }

            break;                        /* found suitable prime */
        }

        cache[i] = cand;
    }

    last_cached = n;
    return cache[n];
}

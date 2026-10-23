/*
 * A140493  Trajectory of 4 under repeated application of the map: n -> n +
 *          third-smallest number that does not divide n.
 * https://oeis.org/A140493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + t(a(n-1)), where t(m) is the third * smallest
 *            positive integer that does *not* divide m (n ≥ 1, * a(1) = 4).
 *            The sequence also satisfies * a(n) = a(n-1) + a(n-12) - a(n-13)
 *            for n > 23
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   third_nondivisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple memoisation cache.  It works
 * for any integer type seq_t (including arbitrary‑precision
 * big integers) and any unsigned integer type idx_t (uint64_t). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/third_nondivisor.h"

seq_t a(idx_t n)
{
    /* Cache previously computed terms: a(1) .. a(stored) */
    static seq_t *seq  = NULL;
    static size_t  cap  = 0;    /* allocated capacity */
    static idx_t   stored = 0;  /* number of terms stored */

    if (n < 1)
        return (seq_t)0;      /* unspecified for n < 1 */

    /* Grow the cache if necessary */
    if (n > stored) {
        size_t needed = (size_t)n;
        if (cap < needed) {
            size_t newcap = cap ? (cap << 1) : 16;
            if (newcap < needed)
                newcap = needed;
            /* In C++ use a cast to seq_t*; this is safe for
             * types with trivial constructors (e.g. integer,
             * __int128).  For arbitrary‑precision classes the
             * builder can provide a trivial allocator. */
            seq_t *tmp = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
            if (!tmp)
                abort();           /* allocation failure */
            seq = tmp;
            cap = newcap;
        }

        /* Compute missing terms */
        for (idx_t k = stored + 1; k <= n; ++k) {
            if (k == 1) {
                seq[0] = (seq_t)4;
            } else {
                seq_t prev = seq[k - 2];
                seq_t add  = third_nondivisor(prev);
                seq[k - 1] = prev + add;
            }
        }
        stored = n;
    }

    return seq[n - 1];
}

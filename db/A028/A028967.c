/*
 * A028967  Theta series of a.c.c. lattice.
 * https://oeis.org/A028967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) such that 4*x^2 - 2*x*y
 *            + 2*x*z + 4*y^2 + 4*y*z + 4*z^2 = 2*n. This is the theta series
 *            of the a.c.c. lattice
 * Range:     u8 n <= 397, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *ac_cache = NULL;
static idx_t ac_cache_size = 0;

/* Recompute the cache up to n (inclusive).  All entries are zero‑initialised
   and then incremented for each lattice vector whose quadratic form equals 2*n. */
static void ac_recompute_cache(idx_t limit_n) {
    size_t sz = (size_t)(limit_n + 1);
    seq_t *new_cache = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_cache) return;          /* out of memory – abort silently */

    /* Integer bound for |x|, |y|, |z|.  For |x|>sqrt(2*limit_n) we have
       4*x^2 > 2*limit_n, so no further solutions are possible. */
    int lim = 0;
    while ((long long)(lim + 1) * (lim + 1) <= 2 * (long long)limit_n) ++lim;

    for (int x = -lim; x <= lim; ++x) {
        for (int y = -lim; y <= lim; ++y) {
            for (int z = -lim; z <= lim; ++z) {
                int s = 4*x*x - 2*x*y + 2*x*z + 4*y*y + 4*y*z + 4*z*z;
                if ((s & 1) == 0) {           /* s must be even */
                    int nval = s >> 1;         /* n = s/2 */
                    if ((idx_t)nval <= limit_n) {
                        new_cache[(size_t)nval] += (seq_t)1;
                    }
                }
            }
        }
    }

    if (ac_cache) free(ac_cache);
    ac_cache = new_cache;
    ac_cache_size = limit_n + 1;
}

seq_t a(idx_t n) {
    if (n + 1 > ac_cache_size) {
        ac_recompute_cache(n);
    }
    return ac_cache[(size_t)n];
}

/*
 * A055525  Shortest other side of a Pythagorean triangle having n as length
 *          of one of the three sides.
 * https://oeis.org/A055525
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = minimal other side of any Pythagorean triangle that contains n
 *  as one of its three sides.  Parameters:
 *
 *  n  : index (>=3)
 *  seq_t : type of the sequence values
 *  idx_t : type of the index (uint64_t)
 *
 *  The function builds a cache of best values up to the largest n
 *  requested so far.  It uses Euclid's formula to generate all primitive
 *  triples whose sides are <= the requested bound, and it scales them to
 *  cover all multiples.  Each time a side is found, the minimal other
 *  side among the remaining two sides is stored as the best value for
 *  that index.  The cache is expanded using realloc; a cast to seq_t*
 *  is required for C++ big‑integer builds.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_sq.h"

/* Compute gcd for idx_t values. */
static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b   = a % b;
        a   = t;
    }
    return a;
}

/* Return the minimal other side of a Pythagorean triangle that contains n. */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Behaviour below the OEIS offset is undefined. */
        return (seq_t)0;
    }

    /* Static cache of best values.  valid indices: [0 .. max_cached] */
    static idx_t max_cached    = 0;
    static seq_t *best         = NULL;

    /* Expand cache to include n on demand. */
    if (n > max_cached) {
        idx_t new_max = n;

        /* Reallocate array of seq_t; cast required for C++ big integer builds. */
        seq_t *tmp = (seq_t *)realloc(best, (new_max + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        best = tmp;

        /* Initialize new entries to zero (default constructed). */
        for (idx_t i = max_cached + 1; i <= new_max; ++i)
            best[i] = (seq_t)0;

        /* Generate all primitive triples with sides <= new_max. */
        idx_t u_limit = new_max / 2 + 2;               /* sufficient upper bound for m */
        for (idx_t u = 2; u <= u_limit; ++u) {
            for (idx_t v = 1; v < u; ++v) {
                if (((u + v) & 1) == 0) continue;    /* opposite parity required */
                if (_gcd_idx(u, v) != 1) continue;    /* coprime required */

                /* Primitive sides as 128-bit integers. */
                __int128 a0_128 = _sq(u) - _sq(v);
                __int128 b0_128 = (__int128)2 * u * v;
                __int128 c0_128 = _sq(u) + _sq(v);
                if (a0_128 <= 0 || b0_128 <= 0 || c0_128 <= 0) continue;

                /* If all primitive sides exceed new_max, further scaling
                 * will also exceed, so skip this pair. */
                if (a0_128 > (__int128)new_max &&
                    b0_128 > (__int128)new_max &&
                    c0_128 > (__int128)new_max)
                    continue;

                idx_t a0 = (idx_t)a0_128;
                idx_t b0 = (idx_t)b0_128;
                idx_t c0 = (idx_t)c0_128;

                /* Scale up by k while at least one side <= new_max. */
                idx_t min_side = a0;
                if (b0 < min_side) min_side = b0;
                if (c0 < min_side) min_side = c0;
                if (min_side == 0) continue;

                idx_t k_max = new_max / min_side;
                for (idx_t k = 1; k <= k_max; ++k) {
                    idx_t a_k = a0 * k;
                    idx_t b_k = b0 * k;
                    idx_t c_k = c0 * k;

                    /* For each side, update the minimal other side. */
                    struct { idx_t side, other1, other2; } sides[3] = {
                        { a_k, b_k, c_k },
                        { b_k, a_k, c_k },
                        { c_k, a_k, b_k }
                    };
                    for (int s = 0; s < 3; ++s) {
                        idx_t side = sides[s].side;
                        if (side < 3) continue;
                        if (side > new_max) continue; /* outside cache */
                        idx_t other1 = sides[s].other1;
                        idx_t other2 = sides[s].other2;
                        idx_t min_other = (other1 < other2) ? other1 : other2;
                        seq_t cand = (seq_t)min_other;
                        if (best[side] == (seq_t)0 || cand < best[side])
                            best[side] = cand;
                    }
                }
            }
        }
        max_cached = new_max;
    }

    /* After ensuring cache up to n, return the stored value. */
    return best[n];
}

/*
 * A063864  Theta series of A15 packing with respect to point (0,1,2).
 * https://oeis.org/A063864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt, is_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the theta series coefficient a(n) for the A15 packing.
   a(n) is the number of integer triples (u,v,w) such that
   u^2 + v^2 + w^2 = n and the residues
   (u mod 4, (v-1) mod 4, (w-2) mod 4) match one of the eight
   residue patterns that arise from the packing translations.
   (0,3,2), (2,1,0), (0,0,0), (0,2,0),
   (2,3,3), (2,3,1), (1,1,2), (3,1,2).
   This is a direct enumeration implementation. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_allowed.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Compute integer square root of n (as seq_t) */
    seq_t ns = (seq_t)n;
    int lim = (int)floor_sqrt(ns);

    for (int ui = -lim; ui <= lim; ++ui) {
        seq_t u2 = (seq_t)ui * (seq_t)ui;
        if (u2 > ns) continue;

        seq_t n_minus_u2 = ns - u2;
        int limV = (int)floor_sqrt(n_minus_u2);

        for (int vi = -limV; vi <= limV; ++vi) {
            seq_t v2 = (seq_t)vi * (seq_t)vi;
            if (u2 + v2 > ns) continue;

            seq_t rem = ns - u2 - v2;
            seq_t w = floor_sqrt(rem);
            if (w * w != rem) continue;          /* remainder not a perfect square */

            if (w == 0) {                        /* only one w */
                if (is_allowed(ui, vi, 0)) count++;
            } else {                              /* both +w and -w */
                int w_int = (int)w;
                if (is_allowed(ui, vi, w_int))   count++;
                if (is_allowed(ui, vi, -w_int))  count++;
            }
        }
    }

    return count;
}

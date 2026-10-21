/*
 * A098546  Irregular triangle T(n,k) = binomial(n,m) where m is the number of
 *          parts in the k-th partition of n listed in Abramowitz and Stegun
 *          order, read by rows, 1 <= k <= A000041(n).
 * https://oeis.org/A098546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = binomial(n, m) where m is the number of parts in the k‑th
 *            partition of n listed in Abramowitz–Stegun order (partitions
 *            grouped by increasing number of parts, then lexicographically)
 * Range:     u8 n <= 54, u16 n <= 1039, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates partitions for each row in the required
   order, counting only the index needed for the queried term.  It is
   efficient for sequence indices up to a few thousand and conforms
   to the type constraints described in the problem statement. */

#include <stddef.h>
#include "../utils/binomial.h"

static int recurse_exact(idx_t rem, idx_t max, idx_t parts_left,
                         idx_t parts_needed,
                         idx_t *counter, idx_t target,
                         idx_t row, seq_t *out)
{
    /* If all parts have been placed, check if the partition sums to rem. */
    if (parts_left == 0) {
        return 0;
    }

    /* If this is the last part to place, it must equal rem and not exceed max. */
    if (parts_left == 1) {
        if (rem <= max) {
            if (*counter == target) {
                *out = binomial(row, parts_needed);
                return 1;
            }
            ++(*counter);
        }
        return 0;
    }

    /* Determine the maximum part that can be taken.
       It must not exceed 'max' and must leave enough for the remaining parts. */
    idx_t upper = rem > max ? max : rem;
    idx_t limit = rem - (parts_left - 1);      /* leave at least 1 for each remaining part */
    if (upper > limit) upper = limit;
    if (upper < 1) return 0;

    /* Generate parts in descending order to preserve lexicographic order
       within each group of equal part count. */
    for (idx_t part = upper; part >= 1; --part) {
        if (recurse_exact(rem - part, part, parts_left - 1,
                          parts_needed, counter, target,
                          row, out))
            return 1;
    }
    return 0;
}

seq_t a(idx_t target)
{
    idx_t counter = 0;    /* Global partition counter across all rows. */
    seq_t  value   = 0;   /* Result to be returned. */

    for (idx_t row = 0; ; ++row) {
        if (row == 0) {          /* The empty partition is the only one for n=0. */
            if (counter == target) {
                value = binomial(0, 0);   /* binomial(0,0) == 1 */
                return value;
            }
            ++counter;
            continue;
        }

        /* For each possible number of parts m, enumerate all partitions. */
        for (idx_t m = 1; m <= row; ++m) {
            if (recurse_exact(row, row, m,
                              m, &counter, target,
                              row, &value))
                return value;
        }
    }
}

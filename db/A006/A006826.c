/*
 * A006826  Erroneous version of A003182.
 * https://oeis.org/A006826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1 (erroneous term); for n >= 1, a(n) = A003182(n), the
 *            number of monotone Boolean functions of n variables up to
 *            permutation of the variables. Computed with Burnside's lemma:
 *            for each permutation of the n variables, count the up-sets of
 *            the poset of subset-orbits by recursion, then average...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   a006826_cnt, a006826_count, a006826_nextperm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006826_cnt.h"
#include "../utils/a006826_count.h"
#include "../utils/a006826_nextperm.h"

seq_t a(idx_t n)
{
    static uint64_t memo[7];
    static int have[7];
    if (n == 0) return (seq_t)1;
    if (n > 6) return (seq_t)0; /* no further terms exist for this dead sequence */
    if (!have[n]) {
        memo[n] = a006826_count((int)n);
        have[n] = 1;
    }
    return (seq_t)memo[n];
}

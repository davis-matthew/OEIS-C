/*
 * A061498  Number of distinct terms in the first difference sequence of
 *          reduced residue system[=dRRS] for n.
 * https://oeis.org/A061498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = number of distinct first differences of the
 *  reduced residue system modulo n.
 *
 *  Implementation: enumerate all numbers 1..n−1 that are coprime to n,
 *  compute successive differences, and count how many distinct
 *  differences occur.  A small boolean array of size n is reused
 *  across calls to avoid repeated allocations.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/my_gcd_idx.h"

seq_t a(idx_t n)
{
    /* For n < 3 the reduced residue system has at most one element,
       therefore there are no differences to compare. */
    if (n <= 2)
        return 0;

    /* static memory to hold seen differences up to the largest n
       seen so far.  Resize if needed. */
    static idx_t max_n_seen = 0;
    static bool *diff_seen = NULL;

    if (n > max_n_seen) {
        diff_seen = (bool *)realloc(diff_seen, (size_t)n * sizeof(bool));
        max_n_seen = n;
    }
    /* clear the difference marker array */
    for (idx_t i = 1; i < n; ++i)
        diff_seen[i] = false;

    seq_t distinct_cnt = 0;
    idx_t prev = 0;
    bool first = true;

    for (idx_t i = 1; i < n; ++i) {
        if (my_gcd_idx(i, n) == 1) {
            if (!first) {
                idx_t d = i - prev;
                if (!diff_seen[d]) {
                    diff_seen[d] = true;
                    ++distinct_cnt;
                }
            }
            prev = i;
            first = false;
        }
    }
    return distinct_cnt;
}

/*
 * A070770  b + c + d where b >= c >= d >= 0 ordered by b then c then d.
 * https://oeis.org/A070770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   comb3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A070770
 *  Formula: a(n) = sum of the triple (b,c,d) that is the nth element in the
 *  lexicographic ordering of all triples of non‑negative integers with
 *  b ≥ c ≥ d ≥ 0, ordered by b, then c, then d   (offset 0).
 *  Equivalently: a(n) = A056556(n) + A056557(n) + A056558(n).
 */

#include <stdint.h>
#include "../utils/comb3.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find the smallest b such that the cumulative number of triples
     * with b' ≤ b exceeds n.  The cumulative count for a given b is C(b+3,3). */
    uint64_t b = 0;
    while (comb3(b + 3) <= n)
        ++b;

    /* index of the first triple with this b is C(b+2,3) */
    uint64_t start   = comb3(b + 2);
    uint64_t offset  = n - start;          /* rank within the block for this b */

    /* locate (c,d) within the block by iterating over c */
    uint64_t c = 0, d = 0;
    for (c = 0; c <= b; ++c) {
        uint64_t cnt = c + 1;             /* number of d‑values for this c */
        if (offset < cnt) {
            d = offset;
            break;
        }
        offset -= cnt;
    }

    /* compute the sum b + c + d and return as seq_t */
    return (seq_t)(b + c + d);
}

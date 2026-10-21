/*
 * A202604  Clique number for the n-Keller graph.
 * https://oeis.org/A202604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A202604 – clique number for the n‑Keller graph.
   For n ≥ 8 the value is 2^n.  The first seven values are
   listed explicitly because no simple closed form is known for them. */

#include <stddef.h>          /* for size_t */
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n) {
    /* Index 0 is unused – the sequence is 1‑based. */
    static const seq_t small[8] = {
        0ULL,        /* unused */
        1ULL,        /* a(1) */
        2ULL,        /* a(2) */
        5ULL,        /* a(3) */
        12ULL,       /* a(4) */
        28ULL,       /* a(5) */
        60ULL,       /* a(6) */
        124ULL       /* a(7) */
    };

    if (n < 8) {
        /* n may be 0 but the problem states n ≥ 1; 0 falls back to 0. */
        return small[(size_t)n];
    }
    /* For n ≥ 8, a(n) = 2^n. */
    return pow2((idx_t)n);
}

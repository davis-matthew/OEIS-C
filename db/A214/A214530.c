/*
 * A214530  List of words over {1,7} with equal numbers of 1's and 7's.
 * https://oeis.org/A214530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Generate the n-th word over {1,7} that has an equal number of 1‑s and 7‑s.
    Words are ordered first by length 2,4,6,… and, within each length,
    lexicographically (equivalently, numerically as base‑10 integers). */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the block (word length 2*k) containing the n-th word. */
    idx_t k = 1;
    seq_t cumulative = 0;
    seq_t comb = 0;
    for (;; ++k) {
        comb = binomial(2 * k, (idx_t)k);     /* C(2k,k) */
        cumulative += comb;
        if (cumulative >= (seq_t)n) break;
    }

    /* Number of words before this block. */
    seq_t prev = cumulative - comb;
    /* Zero‑based index within the current block. */
    idx_t m = (idx_t)((seq_t)n - prev - 1);

    /* Unrank the m‑th combination of positions for 1‑s. */
    seq_t result = 0;
    idx_t ones = k;                 /* Remaining 1‑s to place. */

    for (idx_t pos = 0; pos < 2 * k; ++pos) {
        idx_t remaining_pos = 2 * k - pos - 1;

        if (ones == 0) {
            /* All remaining positions must be 7. */
            result = result * 10 + 7;
            continue;
        }

        /* Count how many words start with a 1 at this position. */
        seq_t count1 = 0;
        if (remaining_pos >= (idx_t)(ones - 1))
            count1 = binomial(remaining_pos, (idx_t)(ones - 1));

        if ((seq_t)m < count1) {          /* Place a 1. */
            result = result * 10 + 1;
            --ones;
        } else {                          /* Place a 7 and skip over the first
                                              count1 words that start with 1. */
            result = result * 10 + 7;
            m -= (idx_t)count1;
        }
    }

    return result;
}

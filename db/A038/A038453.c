/*
 * A038453  Sums of 11 distinct powers of 10.
 * https://oeis.org/A038453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of 11 distinct powers of 10
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   binom, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                    /* undefined for n < 1, but return 0 */

    const idx_t k = 11;              /* number of powers to sum */
    const idx_t r = k - 1;           /* remaining ones after the highest digit */

    /* Find the maximal position m such that the cumulative number of
       combinations up to m-1 is less than n. */
    idx_t m = r;                     /* minimal possible maximum position */
    seq_t cumul = 0;                 /* how many numbers before current block */
    seq_t target = (seq_t)n;

    while (1) {
        seq_t block = binom(m, r);   /* # of combinations with maximum=m */
        if (cumul + block >= target)
            break;
        cumul += block;
        ++m;
    }

    /* Rank of the desired number within the block of fixed maximum m */
    seq_t rank = target - cumul - (seq_t)1;   /* zero‑based rank */

    /* Build the number: start with the highest digit m */
    seq_t result = pow10(m);
    idx_t remaining = r;               /* ones still to place */

    /* Unrank the remaining r ones among positions 0 .. m-1.
       Iterate from high to low positions. */
    for (idx_t pos = m; pos > 0; ) {
        pos--;                          /* current bit position */

        if (remaining == 0)
            break;

        /* Count how many combinations if this position stays 0. */
        seq_t count = binom(pos, remaining);

        /* Decide whether to put a 1 here. */
        if (rank >= count) {
            /* We need to skip all combinations that start with 0 at this
               position, so we place a 1 here. */
            result += pow10(pos);
            rank -= count;
            --remaining;
        }
        /* else: leave this bit 0 and continue to the next lower bit */
    }

    return result;
}

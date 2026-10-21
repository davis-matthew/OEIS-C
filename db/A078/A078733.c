/*
 * A078733  Numbers k such that the b0(k)^2=b1(k) where b1(k) denotes the
 *          number of 1's in binary expansion of k, b0(k) the number of 0's.
 * https://oeis.org/A078733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_capped
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078733: numbers k such that (#0 bits in binary(k))² = (#1 bits in binary(k)) */
/* The algorithm enumerates all possible zero-count values z, uses binomial
   counts to skip complete blocks, then un‑ranks the target combination
   in lexicographic order that corresponds to increasing numeric value.
   It works for any seq_t type that supports integer operators and
   arbitrary‑precision arithmetic. */

#include <stdint.h>
#include "../utils/binom_capped.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not used, offset starts at 1 */

    idx_t cumulative = 0; /* number of terms already counted */
    idx_t z = 0;          /* current zero count */
    idx_t L;              /* binary length */

    /* Find the block (value of z) that contains the n-th term */
    while (1) {
        ++z;
        L = z * (z + 1);            /* length of binary string */
        idx_t remaining = n - cumulative; /* how many more terms needed */
        idx_t cnt = binom_capped(L - 1, z, remaining);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
    }

    /* Position of the target among the terms with zero count z */
    idx_t target = n - cumulative;   /* 1‑based index inside this block */
    idx_t zeros_left = z;

    /* Build the number: start with the leading 1 at position L‑1 */
    seq_t result = ((seq_t)1 << (L - 1));

    /* Process remaining bits from high to low */
    for (int pos = (int)(L - 2); pos >= 0; --pos) {
        /* If no zeros left, all remaining bits are 1 */
        if (zeros_left == 0) {
            result |= ((seq_t)1 << pos);
            continue;
        }

        /* Count how many combinations start with a 0 at this position */
        idx_t rem_pos = pos;  /* positions below current bit */
        idx_t combos0 = binom_capped(rem_pos, zeros_left - 1, target);

        if (target <= combos0) {
            /* Place a 0: consume one zero, leave target unchanged */
            zeros_left -= 1;
        } else {
            /* Place a 1: subtract the skipped 0‑starting combinations */
            result |= ((seq_t)1 << pos);
            target -= combos0;
        }
    }

    return result;
}

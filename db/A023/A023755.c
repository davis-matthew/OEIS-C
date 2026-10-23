/*
 * A023755  Plaindromes: numbers whose digits in base 14 are in nondecreasing
 *          order.
 * https://oeis.org/A023755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th non‑negative integer whose base‑14
 *            representation has nondecreasing digits (most significant
 *            first). The function determines the number’s length, then
 *            un‑ranks the corresponding multiset of digits using binomial
 *            coefficients
 * Range:     u8 n <= 141, u16 n <= 4161, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t factor = (seq_t)(n - k + i);
        result = result * factor / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  n>=1.  We treat the special case n==1 separately
       and then find the length len of the desired number. */
    idx_t remaining = n;
    idx_t len = 1;

    /* Find the length len such that the cumulative count of numbers
       with lengths < len is < n <= cumulative count with length len. */
    while (1) {
        seq_t count;
        if (len == 1) count = 14;                   /* digits 0…13 */
        else           count = binom_idx((idx_t)(len + 12), (idx_t)len);

        if ((seq_t)remaining <= count) break;
        remaining -= (idx_t)count;
        ++len;
    }

    idx_t offset = remaining - 1;   /* 0‑based offset within this length */

    /* Length 1: digits 0…13 are used directly. */
    if (len == 1) {
        return (seq_t)offset;
    }

    /* Length ≥2: generate the digit multiset by combinatorial un‑ranking. */
    seq_t number = 0;
    seq_t rem_k   = (seq_t)offset;
    seq_t prev_d  = 1;                /* first digit cannot be 0 when len>1 */

    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t positions_left = len - pos - 1;

        /* Enumerate possible digits for this position (nondecreasing). */
        for (seq_t d = prev_d; d <= 13; ++d) {
            seq_t comb;
            if (positions_left == 0) {
                comb = 1;                       /* only this digit remains */
            } else {
                /* Number of multisets of size positions_left from digits
                   d…13 (inclusive).  There are (14 - d) distinct values. */
                comb = binom_idx(
                    (idx_t)(positions_left + 13 - d),
                    (idx_t)positions_left
                );
            }

            if (rem_k < comb) {
                /* Choose this digit and move to next position. */
                number = number * 14 + d;
                prev_d = d;
                break;
            } else {
                rem_k -= comb;
            }
        }
    }
    return number;
}

/*
 * A052219  Numbers whose sum of digits is 5.
 * https://oeis.org/A052219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 126, u32 n <= 2001, u64 n <= 9139,
 *            u128 n <= 9139, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th positive integer whose decimal digits sum to 5.
 * The algorithm counts how many such numbers have a given number of
 * digits, finds the appropriate length, and then constructs the
 * n-th number of that length by combinatorial ranking of the digit
 * choices.
 */
static inline uint64_t binom_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined offset */
    uint64_t len = 1;
    uint64_t cum = binom_uint64(len + 4, 5);   /* C(len+4,5) */
    while (cum < (uint64_t)n) {
        ++len;
        cum = binom_uint64(len + 4, 5);
    }
    uint64_t cum_prev = binom_uint64(len + 3, 5);
    uint64_t k = (uint64_t)n - cum_prev;      /* rank within length len (1‑based) */

    uint64_t rem = 5;          /* remaining digit sum to distribute */
    uint64_t value = 0;
    for (uint64_t pos = 1; pos <= len; ++pos) {
        uint64_t start = (pos == 1) ? 1 : 0;
        for (uint64_t d = start; d <= 9 && d <= rem; ++d) {
            uint64_t remaining_positions = len - pos;
            uint64_t count;
            if (remaining_positions == 0) {
                count = (rem == d) ? 1 : 0;
            } else {
                uint64_t rem_after = rem - d;
                count = binom_uint64(rem_after + remaining_positions - 1,
                                     remaining_positions - 1);
            }
            if (k <= count) {
                value = value * 10 + d;
                rem -= d;
                break;
            } else {
                k -= count;
            }
        }
    }
    return (seq_t)value;
}

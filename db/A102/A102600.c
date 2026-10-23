/*
 * A102600  Largest prime which can be formed from digits of n^2, or 0, if no
 *          prime.
 * https://oeis.org/A102600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = A101991(n^2) – largest prime that can be formed from *
 *            the decimal digits of n^2; 0 if no such prime exists
 * Range:     u8 n <= 12, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Recursive generator that builds numbers of a fixed length from the
 * remaining digit multiset (freq).  Digits are tried in descending
 * order so that the generated numbers appear in descending numerical
 * order.  If a prime is found, *best is set and the function returns
 * true to unwind the recursion immediately. */
static bool gen_rec(int depth, int target_len, seq_t cur,
                    int *freq, seq_t *best)
{
    if (depth == target_len) {
        if (cur > 0 && is_prime((uint64_t)cur)) {
            *best = cur;
            return true;
        }
        return false;
    }
    for (int d = 9; d >= 0; --d) {
        if (d == 0 && depth == 0) continue;           /* no leading 0 */
        if (freq[d] == 0) continue;
        --freq[d];
        seq_t next = cur * 10 + d;
        if (gen_rec(depth + 1, target_len, next, freq, best)) {
            ++freq[d];
            return true;
        }
        ++freq[d];
    }
    return false;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Compute n² using seq_t to avoid overflow for larger builds */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Count the decimal digits of sq */
    int freq[10] = {0};
    seq_t tmp = sq;
    int digits = 0;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        ++freq[d];
        ++digits;
        tmp /= 10;
    }

    seq_t best = 0;
    /* Try numbers of decreasing length – longer numbers are always larger */
    for (int len = digits; len >= 1; --len) {
        if (gen_rec(0, len, 0, freq, &best))
            return best;
    }
    return 0;
}

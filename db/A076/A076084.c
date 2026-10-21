/*
 * A076084  Consider all numbers that can be formed by permuting the digits of
 *          n; take those with the greatest number of divisors; a(n) is the
 *          smallest of them.
 * https://oeis.org/A076084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the smallest permutation of the decimal digits of n that
 *            * has the greatest number of divisors among all such
 *            permutations. For ties * the numerically smallest permutation is
 *            chosen
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* Helper: recursively generate all unique permutations of the sorted
 * digit array `digits` of length `len`.  `pos` is the current recursion depth,
 * `cur` is the number built so far.  `best_d` holds the best divisor count
 * seen so far and `best_val` the best permutation value. */
static void gen_rec(const uint8_t *digits, bool *used, int len, int pos,
                    uint64_t cur, idx_t *best_d, seq_t *best_val)
{
    if (pos == len) {
        idx_t d = divisor_count_u64(cur);
        if (d > *best_d || (d == *best_d && cur < (uint64_t)*best_val)) {
            *best_d = d;
            *best_val = (seq_t)cur;
        }
        return;
    }
    for (int i = 0; i < len; ++i) {
        if (used[i]) continue;
        /* skip duplicate digits to avoid generating the same permutation
         * more than once when digits contain repeats */
        if (i > 0 && digits[i] == digits[i-1] && !used[i-1]) continue;
        used[i] = true;
        gen_rec(digits, used, len, pos + 1, cur * 10 + digits[i], best_d, best_val);
        used[i] = false;
    }
}

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into an array in reverse order */
    uint8_t digits[20];          /* supports numbers up to 10^20-1 */
    int len = 0;
    uint64_t tmp = (uint64_t)n;
    if (tmp == 0) { digits[len++] = 0; }
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Sort digits ascending to enable efficient duplicate skipping */
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (digits[i] > digits[j]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    bool used[20] = { false };
    idx_t best_d = 0;
    seq_t best_val = 0;

    gen_rec(digits, used, len, 0, 0ULL, &best_d, &best_val);

    return best_val;
}

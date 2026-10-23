/*
 * A138472  Number of numbers less than n having a greater sum of digits.
 * https://oeis.org/A138472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = #{ m < n : digit_sum(m) > digit_sum(n) } 
 * Implemented by counting, for each n, the numbers m < n with 
 * digit_sum(m) <= digit_sum(n) via digit-DP and taking the complement. 
 */

#include <stdint.h>

#define MAX_DIGITS 20
#define MAX_SUM (9 * MAX_DIGITS)

/* compute digit sum of an idx_t number */
static int digit_sum_idx(idx_t v) {
    int s = 0;
    while (v > 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

/* count numbers <= bound with digit sum <= limit (inclusive) */
static seq_t count_leq(idx_t bound, int limit) {
    /* digit extraction into an array (most significant digit first) */
    int digits[MAX_DIGITS];
    int len = 0;
    if (bound == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (bound > 0) {
            digits[len++] = bound % 10;
            bound /= 10;
        }
        /* reverse to most significant first */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* DP tables: cur[tight][sum] and next[tight][sum] */
    static seq_t cur[2][MAX_SUM + 1];
    static seq_t nxt[2][MAX_SUM + 1];

    /* initialize */
    for (int tight = 0; tight < 2; ++tight)
        for (int sum = 0; sum <= limit; ++sum)
            cur[tight][sum] = 0;
    cur[1][0] = 1;   /* start with tight = 1, sum = 0 */

    /* iterate over digits */
    for (int pos = 0; pos < len; ++pos) {
        /* clear next */
        for (int tight = 0; tight < 2; ++tight)
            for (int sum = 0; sum <= limit; ++sum)
                nxt[tight][sum] = 0;

        int digit_limit = digits[pos];

        for (int tight = 0; tight < 2; ++tight) {
            int tight_digit_limit = tight ? digit_limit : 9;
            for (int sum = 0; sum <= limit; ++sum) {
                seq_t val = cur[tight][sum];
                if (val == 0) continue;
                for (int d = 0; d <= tight_digit_limit; ++d) {
                    int new_sum = sum + d;
                    if (new_sum > limit) break;
                    int new_tight = (tight && (d == digit_limit)) ? 1 : 0;
                    nxt[new_tight][new_sum] += val;
                }
            }
        }
        /* swap cur and nxt */
        for (int tight = 0; tight < 2; ++tight)
            for (int sum = 0; sum <= limit; ++sum)
                cur[tight][sum] = nxt[tight][sum];
    }

    /* sum all states */
    seq_t total = 0;
    for (int tight = 0; tight < 2; ++tight)
        for (int sum = 0; sum <= limit; ++sum)
            total += cur[tight][sum];
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    int s = digit_sum_idx(n);
    seq_t leq = count_leq(n - 1, s);
    return (seq_t)n - leq;
}

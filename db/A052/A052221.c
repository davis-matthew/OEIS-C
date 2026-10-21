/*
 * A052221  Numbers whose sum of digits is 7.
 * https://oeis.org/A052221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose decimal digit sum equals
 *            7
 * Range:     u8 n <= 21, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the number by combinatorial construction. */

#include <stdint.h>

#define MAX_DIGITS 50

static uint64_t ways[MAX_DIGITS + 1][8];
static int ways_initialized = 0;

static void init_ways(void) {
    if (ways_initialized)
        return;
    /* ways[d][s] = # of d‑digit strings (each 0..9) with digit sum s (s ≤ 7) */
    for (int s = 0; s <= 7; ++s)
        ways[0][s] = (s == 0) ? 1 : 0;
    for (int d = 1; d <= MAX_DIGITS; ++d) {
        for (int s = 0; s <= 7; ++s) {
            uint64_t cnt = 0;
            for (int dig = 0; dig <= 9; ++dig) {
                if (dig > s)
                    break;
                cnt += ways[d - 1][s - dig];
            }
            ways[d][s] = cnt;
        }
    }
    ways_initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined for n=0, but avoid UB */

    init_ways();

    /* Precompute counts for each length until we reach n. */
    uint64_t count_len[MAX_DIGITS + 1] = {0};
    count_len[1] = 1;  /* only the number 7 */
    for (int len = 2; len <= MAX_DIGITS; ++len) {
        uint64_t c = 0;
        for (int fd = 1; fd <= 9; ++fd) {
            int rem = 7 - fd;
            if (rem >= 0)
                c += ways[len - 1][rem];
        }
        count_len[len] = c;
    }

    /* Find the length of the n-th number. */
    uint64_t cumulative = 0;
    int len = 0;
    for (int l = 1; l <= MAX_DIGITS; ++l) {
        if (cumulative + count_len[l] >= n) {
            len = l;
            break;
        }
        cumulative += count_len[l];
    }
    if (len == 0)
        return 0;  /* n too large for the fixed MAX_DIGITS */

    uint64_t offset = n - cumulative - 1;  /* 0‑based offset within this length */
    int sum_rem = 7;
    seq_t value = 0;

    for (int pos = 0; pos < len; ++pos) {
        int min_d = (pos == 0) ? 1 : 0;
        for (int d = min_d; d <= 9; ++d) {
            if (sum_rem < d)
                continue;
            int rem_digits = len - pos - 1;
            uint64_t cnt = ways[rem_digits][sum_rem - d];
            if (offset < cnt) {
                value = value * (seq_t)10 + (seq_t)d;
                sum_rem -= d;
                break;
            } else {
                offset -= cnt;
            }
        }
    }
    return value;
}

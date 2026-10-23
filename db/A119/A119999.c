/*
 * A119999  Number of partitions of n into parts that occur in decimal
 *          representation as substrings of n.
 * https://oeis.org/A119999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   Counts the number of unrestricted partitions of n whose parts
 *            are numbers that appear as contiguous decimal substrings of n
 *            (excluding 0). The algorithm enumerates all distinct positive
 *            substring values of n, then counts coin‑change combinations
 *            (unlimited supply of each part)
 * Range:     u8 n <= 101, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Enumerate unique positive substrings of n */
    idx_t parts[20];   /* maximum 20 substrings for n < 2^64 */
    idx_t part_cnt = 0;

    /* Extract digits of n into an array `digits[msd]` */
    idx_t tmp = n;
    unsigned digit_rev[20];
    idx_t rev_len = 0;
    if (tmp == 0) {
        digit_rev[0] = 0;
        rev_len = 1;
    } else {
        while (tmp > 0) {
            digit_rev[rev_len++] = tmp % 10;
            tmp /= 10;
        }
    }
    unsigned digit[20];
    for (idx_t i = 0; i < rev_len; ++i)
        digit[i] = digit_rev[rev_len - 1 - i];
    idx_t len = rev_len;

    /* Generate all distinct positive substrings */
    for (idx_t start = 0; start < len; ++start) {
        unsigned val = 0;
        for (idx_t end = start; end < len; ++end) {
            val = val * 10 + digit[end];
            if (val == 0) continue;          /* ignore the part 0 */
            /* check for duplicates */
            int dup = 0;
            for (idx_t k = 0; k < part_cnt; ++k)
                if (parts[k] == val) { dup = 1; break; }
            if (!dup) parts[part_cnt++] = val;
        }
    }

    /* Dynamic programming: dp[s] = number of ways to make sum s */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failure */
    dp[0] = 1;
    for (idx_t i = 0; i < part_cnt; ++i) {
        idx_t p = parts[i];
        for (idx_t s = p; s <= n; ++s)
            dp[s] += dp[s - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

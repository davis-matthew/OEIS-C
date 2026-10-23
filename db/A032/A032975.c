/*
 * A032975  Numbers with the property that all pairs of consecutive base-4
 *          digits differ by 0 or 1.
 * https://oeis.org/A032975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the nth integer whose base‑4 representation has no
   consecutive digits differing by more than 1.
   Computed by dynamic programming and prefix counting. */

#include <limits.h>

seq_t a(idx_t n) {
    /* n is 1‑indexed. For n = 0 return 0 (unused). */
    if (n == 0) return (seq_t)0;

    /* Maximum length needed for 64‑bit n is well below 128 digits. */
    const int MAXLEN = 128;
    /* dp[len][prev] = number of admissible tails of length len
       given previous digit prev (0‑3).  dp[0][*] = 1. */
    unsigned long long dp[MAXLEN + 1][4];
    for (int prev = 0; prev < 4; ++prev) dp[0][prev] = 1ULL;

    int len = 1;
    unsigned __int128 cumulative = 0;   /* count of numbers with fewer than len digits */

    for (;;) {
        /* compute dp for this length */
        for (int prev = 0; prev < 4; ++prev) {
            unsigned __int128 sum = 0;
            int low  = (prev > 0) ? prev - 1 : 0;
            int high = (prev < 3) ? prev + 1 : 3;
            for (int d = low; d <= high; ++d) sum += dp[len - 1][d];
            dp[len][prev] = (sum > ULLONG_MAX) ? ULLONG_MAX : (unsigned long long)sum;
        }

        /* total numbers of exactly this length */
        unsigned __int128 count_len = 0;
        for (int d = 1; d <= 3; ++d) count_len += dp[len - 1][d];

        if (cumulative + count_len >= n) break;  /* target length found */
        cumulative += count_len;
        ++len;
    }

    unsigned long long idx_in_len = (unsigned long long)(n - (unsigned long long)cumulative);
    unsigned long long idx       = idx_in_len;

    seq_t value = 0;
    int remaining_len = len - 1;
    int prev_digit = -1;

    for (int pos = 0; pos < len; ++pos) {
        int low, high;
        if (pos == 0) {
            low = 1;  high = 3;            /* first digit cannot be 0 */
        } else {
            low  = (prev_digit > 0) ? prev_digit - 1 : 0;
            high = (prev_digit < 3) ? prev_digit + 1 : 3;
        }

        int chosen = -1;
        for (int d = low; d <= high; ++d) {
            unsigned long long cnt = dp[remaining_len][d];
            if (idx <= cnt) {
                chosen = d;
                break;
            } else {
                idx -= cnt;
            }
        }
        /* build the value in base 4 */
        value = value * (seq_t)4 + (seq_t)chosen;
        prev_digit = chosen;
        --remaining_len;
    }

    return value;
}

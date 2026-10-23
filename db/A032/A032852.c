/*
 * A032852  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032852 – numbers whose base‑5 digits satisfy
 *   d_m ≤ d_{m-1} ≥ d_{m-2} ≤ d_{m-3} …
 *
 * The implementation enumerates admissible base‑5 strings of
 * increasing length using dynamic programming for counting
 * completions, then selects the k‑th string in numeric order
 * for the requested index n.
 * It works with any integer type seq_t supplied by the build
 * (including arbitrary‑precision integers).
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_LEN 32   /* sufficient for n up to many thousands */

/* DP tables: dp_known[rem][prev][ge] */
static bool dp_known[MAX_LEN + 1][5][2];
static uint64_t dp_val[MAX_LEN + 1][5][2];

/* Count of admissible suffixes of length 'rem' that start
 * with previous digit 'prev'.  The variable 'ge' (true) means
 * the next digit must be greater than or equal to 'prev'.
 * If 'ge' is false, the next digit must be less than or equal to 'prev'. */
static uint64_t count_dp(int rem, int prev, bool ge) {
    if (dp_known[rem][prev][ge]) return dp_val[rem][prev][ge];
    uint64_t res;
    if (rem == 0) {
        res = 1;
    } else {
        int start = ge ? prev : 0;
        int end   = ge ? 4  : prev;
        res = 0;
        for (int d = start; d <= end; ++d) {
            res += count_dp(rem - 1, d, !ge);
        }
    }
    dp_known[rem][prev][ge] = true;
    dp_val[rem][prev][ge]   = res;
    return res;
}

/* Number of admissible base‑5 strings of length 'len'. */
static uint64_t count_len(int len) {
    uint64_t total = 0;
    for (int d = 1; d <= 4; ++d) {
        /* after the first digit the next comparison is '≥' (ge=true) */
        total += count_dp(len - 1, d, true);
    }
    return total;
}

/* Sequence accessor */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for offset 1 – safety */

    /* Find the length of the number at position n. */
    uint64_t cumulative = 0;
    int len = 1;
    for (;;) {
        uint64_t len_count = count_len(len);
        if (cumulative + len_count >= n) break;
        cumulative += len_count;
        ++len;
    }

    /* Index within numbers of this length (0‑based). */
    uint64_t target = n - cumulative - 1;

    seq_t result = 0;
    int   prev   = 0;   /* previous digit, irrelevant for first digit */
    bool  ge     = true;/* next comparison must be '≥' after first digit */
    int   rem    = len; /* digits remaining to place, including MSB */

    while (rem > 0) {
        if (rem == len) {            /* first digit – no constraint */
            for (int d = 1; d <= 4; ++d) {
                uint64_t cnt = count_dp(rem - 1, d, true); /* next must be '≥' */
                if (target >= cnt) {
                    target -= cnt;
                } else {
                    result = result * ((seq_t)5) + (seq_t)d;
                    prev   = d;
                    ge     = true;                     /* next comparison is '≥' */
                    break;
                }
            }
        } else {                     /* subsequent digits – use current 'ge' */
            int start = ge ? prev : 0;
            int end   = ge ? 4   : prev;
            for (int d = start; d <= end; ++d) {
                uint64_t cnt = count_dp(rem - 1, d, !ge);
                if (target >= cnt) {
                    target -= cnt;
                } else {
                    result = result * ((seq_t)5) + (seq_t)d;
                    prev   = d;
                    ge     = !ge;                     /* toggle comparison */
                    break;
                }
            }
        }
        --rem;
    }

    return result;
}

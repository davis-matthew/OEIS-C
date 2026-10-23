/*
 * A043578  Numbers whose base-2 representation has exactly 11 runs.
 * https://oeis.org/A043578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043578 – numbers whose binary representation has exactly
   11 runs (alternating runs of 1s and 0s, starting with a 1).
   The algorithm enumerates binary strings of increasing length L
   and, for a given L, splits the string into 11 positive run lengths
   in the order that corresponds to ascending integer values.
   Complexity: O(L + 11) per call. */
#include <stdint.h>
#include <stdbool.h>

static seq_t binom_seq(uint64_t n, uint64_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find minimal length L such that C(L,11) >= n */
    uint64_t L = 11;
    while (1) {
        seq_t cnt = binom_seq(L, 11);
        if ((idx_t)cnt >= n) break;
        ++L;
    }

    /* Rank within numbers of length L (0‑based) */
    seq_t total_less = binom_seq(L - 1, 11);
    seq_t rank = (seq_t)(n - total_less) - 1;   /* n is >=1, rank < binom_seq(L,11) */

    uint64_t rem_bits = L;
    seq_t result = (seq_t)0;

    for (uint64_t run = 1; run <= 11; ++run) {
        uint64_t runs_left = 12 - run;                 /* including current run */
        uint64_t min_len = 1;
        uint64_t max_len = rem_bits - (runs_left - 1);

        uint64_t len;
        bool is_one_run = (run % 2 == 1);

        if (runs_left == 1) {                         /* last run */
            len = rem_bits;
        } else if (is_one_run) {                      /* 1‑runs: iterate ascending */
            for (len = min_len; len <= max_len; ++len) {
                seq_t cnt = binom_seq(rem_bits - len - 1,
                                      runs_left - 2);
                if (rank < cnt) break;
                rank -= cnt;
            }
        } else {                                      /* 0‑runs: iterate descending */
            for (len = max_len; ; --len) {
                seq_t cnt = binom_seq(rem_bits - len - 1,
                                      runs_left - 2);
                if (rank < cnt) break;
                rank -= cnt;
                if (len == min_len) break;            /* safety, should not happen */
            }
        }

        /* Append this run to the lower bits of result. */
        result <<= len;
        if (is_one_run) {
            result |= ((seq_t)1 << len) - (seq_t)1;
        }
        rem_bits -= len;
    }

    return result;
}

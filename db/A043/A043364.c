/*
 * A043364  Numbers having four 3's in base 5.
 * https://oeis.org/A043364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   comb_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers having exactly four 3's in base‑5. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/comb_n_k.h"

/* 4 ^ e, e is small (≤ ~20). */
static inline uint64_t pow4(uint64_t e)
{
    uint64_t r = 1;
    while (e--) r *= 4;
    return r;
}

/* number of ways to fill rem_pos positions with exactly need_3 copies of 3
   (remaining digits may be 0,1,2,4). */
static inline uint64_t count_completions(uint64_t rem_pos, uint64_t need_3)
{
    if (need_3 > rem_pos) return 0;
    return comb_n_k(rem_pos, need_3) * pow4(rem_pos - need_3);
}

/* number of valid length‑L strings (first digit ≠0) that contain exactly four 3’s */
static inline uint64_t count_len(uint64_t L)
{
    if (L == 4) return 1;
    uint64_t total   = count_completions(L, 4);
    uint64_t leading = count_completions(L - 1, 4); /* leading zero strings */
    return total - leading;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint64_t L = 4;
    uint64_t cumulative = 0;
    uint64_t count = 0;

    /* locate the length containing the nth term */
    for (;; ++L) {
        count = (L == 4) ? 1 : count_len(L);
        cumulative += count;
        if (n <= cumulative) break;
    }

    uint64_t prev_cumulative = cumulative - count;
    uint64_t idx_in_L = n - prev_cumulative;   /* 1‑based index inside this length */

    uint64_t tmp_val = 0;          /* use 64‑bit to build the value */
    uint64_t need_3 = 4;
    bool first_pos = true;

    for (uint64_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 4; ++d) {
            if (first_pos && d == 0) continue;          /* no leading zero */

            uint64_t new_need_3 = need_3;
            if (d == 3) {
                if (need_3 == 0) continue;              /* no more 3’s allowed */
                new_need_3--;
            }

            uint64_t rem_pos = L - pos - 1;
            if (new_need_3 > rem_pos) continue;          /* cannot fit remaining 3's */

            uint64_t completions = count_completions(rem_pos, new_need_3);

            if (idx_in_L <= completions) {
                tmp_val = tmp_val * 5 + (uint64_t)d;
                need_3 = new_need_3;
                first_pos = false;
                break;                                   /* go to next position */
            } else {
                idx_in_L -= completions;
            }
        }
    }

    return (seq_t)tmp_val;
}

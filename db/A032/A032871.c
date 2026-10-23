/*
 * A032871  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A032871: numbers whose base‑8 representation d(m)…d(0)
   satisfies d(m) ≥ d(m-1) ≤ d(m-2) ≥ … .  */
#include <stdint.h>

/* count_rem[rem][prev][dir] is the number of ways to fill 'rem'
   remaining positions after having placed digit 'prev' and
   with the next comparison direction 'dir'.
   dir = 0  → next digit must be ≤ prev (prev ≥ next)
   dir = 1  → next digit must be ≥ prev (prev ≤ next)  */
static uint64_t count_rem[21][8][2];
static int count_precomputed = 0;

static void precompute_counts(void) {
    if (count_precomputed) return;
    count_precomputed = 1;

    /* base case: no more digits to place → exactly one completion */
    for (int prev = 0; prev < 8; ++prev) {
        count_rem[0][prev][0] = 1;
        count_rem[0][prev][1] = 1;
    }

    /* DP for remaining > 0 */
    for (int rem = 1; rem <= 20; ++rem) {
        for (int prev = 0; prev < 8; ++prev) {
            for (int dir = 0; dir < 2; ++dir) {
                uint64_t sum = 0;
                if (dir == 0) {                 /* prev ≥ next → next ≤ prev */
                    for (int nd = 0; nd <= prev; ++nd)
                        sum += count_rem[rem - 1][nd][1];
                } else {                         /* prev ≤ next → next ≥ prev */
                    for (int nd = prev; nd < 8; ++nd)
                        sum += count_rem[rem - 1][nd][0];
                }
                count_rem[rem][prev][dir] = sum;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* safety, offset starts at 1 */

    precompute_counts();

    uint64_t target = (uint64_t)n;
    uint64_t cumulative = 0;
    int len = 1;

    /* find the length of the nth term */
    while (1) {
        uint64_t len_count = 0;
        for (int first = 1; first <= 7; ++first)
            len_count += count_rem[len - 1][first][0];
        if (target <= cumulative + len_count) break;
        cumulative += len_count;
        ++len;
    }

    uint64_t offset = target - cumulative;   /* 1‑based offset in this length */

    /* build the number digit by digit */
    seq_t value = 0;
    int remaining = len - 1;  /* digits still to place after the most‑significant one */
    int dir = 0;              /* 0: next ≤ prev ; 1: next ≥ prev */
    int prev_digit = 0;       /* placeholder before first digit is chosen */

    /* choose the first (most‑significant) digit */
    for (int d = 1; d <= 7; ++d) {
        uint64_t cnt = count_rem[remaining][d][dir];
        if (offset > cnt) {
            offset -= cnt;
        } else {
            value = (seq_t)d;
            prev_digit = d;
            /* remaining stays as len-1 for the following loop */
            break;
        }
    }

    /* choose the remaining digits */
    while (remaining > 0) {
        if (dir == 0) {  /* next ≤ prev_digit */
            for (int d = 0; d <= prev_digit; ++d) {
                uint64_t cnt = count_rem[remaining - 1][d][dir ^ 1];
                if (offset > cnt) {
                    offset -= cnt;
                } else {
                    value = value * (seq_t)8 + (seq_t)d;
                    prev_digit = d;
                    remaining--;
                    dir ^= 1;
                    break;
                }
            }
        } else {          /* dir == 1 : next ≥ prev_digit */
            for (int d = prev_digit; d <= 7; ++d) {
                uint64_t cnt = count_rem[remaining - 1][d][dir ^ 1];
                if (offset > cnt) {
                    offset -= cnt;
                } else {
                    value = value * (seq_t)8 + (seq_t)d;
                    prev_digit = d;
                    remaining--;
                    dir ^= 1;
                    break;
                }
            }
        }
    }

    return value;
}

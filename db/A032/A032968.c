/*
 * A032968  Numbers with the property that all pairs of consecutive base-5
 *          digits differ by more than 1.
 * https://oeis.org/A032968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ok_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction: nth term of A032968 – numbers whose consecutive base‑5 digits differ by more than 1. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ok_digit.h"

/* Precomputed DP tables (size sufficient for 64‑bit indices). */
#define MAXLEN 200
static uint64_t dp[MAXLEN+1][5];
static uint64_t count_len[MAXLEN+1];
static uint64_t cumulative[MAXLEN+1];
static bool prepared = false;

/* Build tables once. */
static void prepare(void) {
    if (prepared) return;
    prepared = true;
    /* dp[0][prev] = 1 for all prev */
    for (int d = 0; d < 5; ++d) dp[0][d] = 1ULL;
    cumulative[0] = 0ULL;

    for (int len = 1; len <= MAXLEN; ++len) {
        /* dp[len][prev] */
        for (int prev = 0; prev < 5; ++prev) {
            uint64_t sum = 0ULL;
            for (int next = 0; next < 5; ++next)
                if (ok_digit(prev, next))
                    sum += dp[len-1][next];
            dp[len][prev] = sum;
        }

        /* numbers of exactly this length (no leading zero) */
        uint64_t cnt = 0ULL;
        for (int first = 1; first <= 4; ++first)
            cnt += dp[len-1][first];
        count_len[len] = cnt;
        cumulative[len] = cumulative[len-1] + cnt;
    }
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    prepare();

    /* find length L such that cumulative[L-1] < n <= cumulative[L] */
    int L = 1;
    while (L <= MAXLEN && (uint64_t)n > cumulative[L]) ++L;
    if (L > MAXLEN) return (seq_t)0;  /* n too large for this precomputation */

    uint64_t rem = (uint64_t)n - cumulative[L-1]; /* 1‑based position within length L */

    seq_t value = 0;
    int prev = -1;
    for (int d = 1; d <= 4; ++d) {
        uint64_t cnt = dp[L-1][d];
        if (cnt < rem) {
            rem -= cnt;
        } else {
            value = d;
            prev = d;
            int left = L - 1;
            while (left > 0) {
                for (int nxt = 0; nxt < 5; ++nxt) {
                    if (!ok_digit(prev, nxt)) continue;
                    uint64_t cnt2 = dp[left-1][nxt];
                    if (cnt2 < rem) {
                        rem -= cnt2;
                    } else {
                        value = value * 5 + (seq_t)nxt;
                        prev = nxt;
                        --left;
                        break;
                    }
                }
            }
            return value;
        }
    }

    /* unreachable */
    return value;
}

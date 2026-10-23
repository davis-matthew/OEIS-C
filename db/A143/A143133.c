/*
 * A143133  Number of cribbage hands with score n when flush points are
 *          omitted.
 * https://oeis.org/A143133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, changed
 * Formula:   A143133 - number of cribbage hands with score n when flush
 *            points are omitted
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t *counts = NULL;
    static idx_t capacity = 0;
    static idx_t max_score = 0;

    if (!init) {
        /* allocate initial storage for score counts */
        capacity = 64;
        counts = (seq_t*)malloc(sizeof(seq_t) * capacity);
        memset(counts, 0, sizeof(seq_t) * capacity);

        /* precompute all combinations of 4 distinct hand cards */
        for (idx_t c0 = 0; c0 < 52 - 3; ++c0) {
            for (idx_t c1 = c0 + 1; c1 < 52 - 2; ++c1) {
                for (idx_t c2 = c1 + 1; c2 < 52 - 1; ++c2) {
                    for (idx_t c3 = c2 + 1; c3 < 52; ++c3) {
                        /* hand card data */
                        idx_t hand_rank[4];
                        idx_t hand_suit[4];
                        idx_t hand_val[4];
                        idx_t hand_idx[4] = {c0, c1, c2, c3};
                        for (idx_t i = 0; i < 4; ++i) {
                            idx_t card = hand_idx[i];
                            hand_rank[i] = card / 4;        /* 0 .. 12 */
                            hand_suit[i] = card % 4;        /* 0 .. 3 */
                            hand_val[i] = (hand_rank[i] >= 9 ? 10 : hand_rank[i] + 1);
                        }

                        /* iterate over all possible starter cards */
                        for (idx_t sc = 0; sc < 52; ++sc) {
                            if (sc == c0 || sc == c1 || sc == c2 || sc == c3) continue;

                            idx_t starter_rank = sc / 4;
                            idx_t starter_suit = sc % 4;
                            idx_t starter_val  = (starter_rank >= 9 ? 10 : starter_rank + 1);

                            /* build arrays for the 5-card hand */
                            idx_t r[5] = {hand_rank[0], hand_rank[1], hand_rank[2], hand_rank[3], starter_rank};
                            idx_t v[5] = {hand_val[0],   hand_val[1],   hand_val[2],   hand_val[3],   starter_val};
                            idx_t s[5] = {hand_suit[0], hand_suit[1], hand_suit[2], hand_suit[3], starter_suit};

                            idx_t score = 0;

                            /* 15s - count all subsets of the 5 cards that sum to 15 */
                            for (idx_t mask = 1; mask < 32; ++mask) {
                                idx_t sum = 0;
                                if (mask & 1)   sum += v[0];
                                if (mask & 2)   sum += v[1];
                                if (mask & 4)   sum += v[2];
                                if (mask & 8)   sum += v[3];
                                if (mask & 16)  sum += v[4];
                                if (sum == 15)  score += 2;
                            }

                            /* pairs - each pair scores 2 points */
                            idx_t freq_pair[13] = {0};
                            for (idx_t i = 0; i < 5; ++i) freq_pair[r[i]]++;
                            for (idx_t k = 0; k < 13; ++k) {
                                if (freq_pair[k] > 1) {
                                    idx_t f = freq_pair[k];
                                    score += (f * (f - 1) / 2) * 2;
                                }
                            }

                            /* runs - find longest run(s) of consecutive ranks */
                            idx_t freq_run[13] = {0};
                            for (idx_t i = 0; i < 5; ++i) freq_run[r[i]]++;
                            idx_t maxL = 0;
                            idx_t i = 0;
                            while (i < 13) {
                                if (freq_run[i] == 0) { ++i; continue; }
                                idx_t j = i;
                                while (j < 13 && freq_run[j] > 0) ++j;
                                idx_t len = j - i;
                                if (len > maxL) maxL = len;
                                i = j;
                            }
                            if (maxL >= 3) {
                                idx_t run_sum = 0;
                                i = 0;
                                while (i < 13) {
                                    if (freq_run[i] == 0) { ++i; continue; }
                                    idx_t j = i;
                                    while (j < 13 && freq_run[j] > 0) ++j;
                                    idx_t len = j - i;
                                    if (len == maxL) {
                                        idx_t prod = 1;
                                        for (idx_t k = i; k < j; ++k) prod *= freq_run[k];
                                        run_sum += maxL * prod;
                                    }
                                    i = j;
                                }
                                score += run_sum;
                            }

                            /* nobs - 1 point for a jack in hand matching starter suit */
                            for (idx_t h = 0; h < 4; ++h) {
                                if (r[h] == 10 && s[h] == starter_suit) { /* rank 10 is Jack */
                                    score += 1;
                                    break;
                                }
                            }

                            /* store count for this score (flush points deliberately omitted) */
                            if (score >= capacity) {
                                idx_t new_cap = capacity;
                                while (score >= new_cap) new_cap *= 2;
                                counts = (seq_t*)realloc(counts, sizeof(seq_t) * new_cap);
                                memset(counts + capacity, 0, sizeof(seq_t) * (new_cap - capacity));
                                capacity = new_cap;
                            }
                            counts[score] += 1;
                            if (score > max_score) max_score = score;
                        }
                    }
                }
            }
        }
        init = true;
    }

    if (n > max_score) return 0;
    return counts[n];
}

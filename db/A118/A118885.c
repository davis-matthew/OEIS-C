/*
 * A118885  Number of binary sequences of length n containing exactly one
 *          subsequence 0011.
 * https://oeis.org/A118885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n containing exactly
 *            one substring 0011. The sequence is computed with a
 *            finite‑automaton DP that counts strings with 0 or 1 occurrences
 *            while iterating over the length
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *best = NULL;          /* memoised results a(0)..a(max_n)   */
static idx_t best_len = 0;          /* allocated length of best array     */
static idx_t computed_len = 0;      /* highest n for which results done   */
static seq_t dp0[5], dp1[5];        /* DP for strings with 0 and 1 occ.   */
static seq_t ndp0[5], ndp1[5];      /* temporary DP arrays               */
static int trans[5][2];
static bool incFlag[5][2];
static bool initialized = false;

static void build_transitions(void)
{
    const unsigned pat[4] = {0,0,1,1};
    for (int s = 0; s <= 4; ++s) {
        for (int b = 0; b < 2; ++b) {
            int max_t = s + 1;
            if (max_t > 4) max_t = 4;
            int best_t = 0;
            for (int t = max_t; t >= 0; --t) {
                bool ok = true;
                for (int k = 0; k < t; ++k) {
                    int pos = (s + 1) - t + k;
                    int val;
                    if (pos < s)
                        val = pat[pos];
                    else /* pos == s */
                        val = b;
                    if (val != pat[k]) { ok = false; break; }
                }
                if (ok) { best_t = t; break; }
            }
            trans[s][b] = best_t;
            incFlag[s][b] = (best_t == 4);
        }
    }
}

static void extend_to(idx_t n)
{
    if (n <= computed_len) return;
    for (; computed_len < n; ++computed_len) {
        for (int i = 0; i < 5; ++i) { ndp0[i] = ndp1[i] = 0; }
        for (int s = 0; s < 5; ++s) {
            for (int b = 0; b < 2; ++b) {
                int t   = trans[s][b];
                bool inc = incFlag[s][b];
                seq_t v0 = dp0[s];
                seq_t v1 = dp1[s];
                if (v0) {
                    if (inc) ndp1[t] += v0;
                    else     ndp0[t] += v0;
                }
                if (v1) {
                    if (!inc) ndp1[t] += v1; /* ignore if a second occ. */
                }
            }
        }
        for (int i = 0; i < 5; ++i) { dp0[i] = ndp0[i]; dp1[i] = ndp1[i]; }
        seq_t sum = 0;
        for (int i = 0; i < 5; ++i) sum += dp1[i];
        if (best_len <= computed_len + 1) {
            size_t new_len = best_len ? best_len * 2 : 8;
            while (new_len <= computed_len + 1) new_len *= 2;
            best = (seq_t *)realloc(best, sizeof(seq_t) * new_len);
            best_len = new_len;
        }
        best[computed_len + 1] = sum;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise DP for empty string */
        dp0[0] = 1;
        for (int i = 1; i < 5; ++i) dp0[i] = 0;
        for (int i = 0; i < 5; ++i) dp1[i] = 0;

        /* memoise a(0) = 0 */
        best_len = 1;
        best = (seq_t *)malloc(sizeof(seq_t));
        best[0] = 0;

        build_transitions();
        computed_len = 0;
        initialized = true;
    }
    extend_to(n);
    return best[n];
}

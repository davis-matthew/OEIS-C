/*
 * A207850  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over columns. For n=1:
 *            number of admissible columns S=42 (vertical constraints only).
 *            For n=2: all pairs of admissible columns, a(2)=S*S. For n≥3: DP
 *            over state (prev2,prev1) using the fact that the horizontal
 *            restriction reduces to forbidding two consecutive...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Number of admissible 7‑bit columns (vertical restrictions only) */
#define S 42
#define FULL 127
#define PAIRS (S * S)

/* Store admissible column masks */
static uint8_t col[S];
/* For each column b, list of columns a such that (a|b)==FULL */
static int match_idx[S][S];
static int match_cnt[S];

/* Initialise admissible columns and pre‑computed match lists once. */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Build admissible columns: vertical patterns 000 and 111 forbidden */
    int mcnt = 0;
    for (int m = 0; m < 128; ++m) {
        bool ok = true;
        for (int i = 0; i <= 4; ++i) {
            int sub = (m >> i) & 7;          /* 3 consecutive rows */
            if (sub == 0 || sub == 7) {      /* 000 or 111 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            col[mcnt++] = (uint8_t)m;
        }
    }
    /* mcnt should be S (42). */
    (void)mcnt;

    /* Pre‑compute, for each b, the list of a with (a|b)==FULL */
    for (int b = 0; b < S; ++b) {
        int cnt = 0;
        int bm = col[b];
        for (int a = 0; a < S; ++a) {
            if ((col[a] | bm) == FULL) {
                match_idx[b][cnt++] = a;
            }
        }
        match_cnt[b] = cnt;
    }
}

seq_t a(idx_t n)
{
    init();

    if (n == 1) return (seq_t)S;
    if (n == 2) return (seq_t)S * (seq_t)S;

    /* DP arrays (state = (prev2,prev1) as a pair of column indices) */
    static seq_t dp[PAIRS];
    static seq_t nextdp[PAIRS];

    /* Initialise for length 2: each pair appears once */
    for (int i = 0; i < PAIRS; ++i) dp[i] = 1;

    for (idx_t len = 3; len <= n; ++len) {
        /* For each middle column b compute sum over valid a */
        for (int b = 0; b < S; ++b) {
            seq_t sum = 0;
            for (int k = 0; k < match_cnt[b]; ++k) {
                int a = match_idx[b][k];
                sum += dp[a * S + b];
            }
            /* Set nextdp for every possible right column c */
            int base = b * S;
            for (int c = 0; c < S; ++c) {
                nextdp[base + c] = sum;
            }
        }
        /* Copy nextdp to dp for next iteration */
        for (int i = 0; i < PAIRS; ++i) dp[i] = nextdp[i];
    }

    /* Sum all states for final length */
    seq_t total = 0;
    for (int i = 0; i < PAIRS; ++i) total += dp[i];
    return total;
}

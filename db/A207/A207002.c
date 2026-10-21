/*
 * A207002  Number of 5Xn 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A207002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over columns using a transfer matrix.
 * The sequence counts 5xN binary matrices avoiding horizontal pattern 010
 * and vertical pattern 011.  For N≥3 we store the last two columns
 * (a 10‑bit state) and transition by adding a new column that
 * does not create forbidden horizontal triples.  The vertical
 * restriction is enforced by only allowing column vectors that
 * do not contain 011 in their 5 bits.  The DP is O(k³·N) with
 * k = |allowed columns| = 20. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static tables initialised once */
    static int init = 0;
    static uint8_t allowed[32];             /* 1 if column is admissible */
    static int allowed_vecs[32];            /* indices of admissible columns */
    static int k = 0;                       /* number of admissible columns   */
    static uint8_t bits[32][5];             /* bits[v][r] : bit r of column v */
    static int trans_to[1024][32];          /* adjacency list for states */
    static int trans_count[1024];           /* number of outgoing edges per state */
    static seq_t dp[1024];
    static seq_t dp_next[1024];

    if (!init) {
        /* pre‑compute bits of every 5‑bit vector */
        for (int v = 0; v < 32; ++v) {
            for (int r = 0; r < 5; ++r)
                bits[v][r] = (uint8_t)((v >> r) & 1);
        }

        /* determine admissible column vectors (no vertical 011) */
        k = 0;
        for (int v = 0; v < 32; ++v) {
            int ok = 1;
            for (int r = 0; r <= 2; ++r) {
                if (!bits[v][r] && bits[v][r + 1] && bits[v][r + 2]) {
                    ok = 0; break;
                }
            }
            allowed[v] = ok;
            if (ok) allowed_vecs[k++] = v;
        }

        /* build transition table over state (prev2,prev1) → (prev1,cur) */
        for (int s = 0; s < 1024; ++s) trans_count[s] = 0;
        for (int a = 0; a < 32; ++a) if (allowed[a]) {
            for (int b = 0; b < 32; ++b) if (allowed[b]) {
                int s = a * 32 + b;
                for (int c = 0; c < 32; ++c) if (allowed[c]) {
                    int ok = 1;
                    for (int r = 0; r < 5; ++r) {
                        if (!bits[a][r] && bits[b][r] && !bits[c][r]) {
                            ok = 0; break;
                        }
                    }
                    if (ok) {
                        int ns = b * 32 + c;
                        trans_to[s][trans_count[s]++] = ns;
                    }
                }
            }
        }
        init = 1;
    }

    if (n == 1)                 /* single column */
        return (seq_t)k;

    if (n == 2) {               /* two columns, no horizontal triples yet */
        seq_t t = (seq_t)k;
        return t * t;
    }

    /* n ≥ 3 : initialise dp with all pairs of admissible columns */
    for (int s = 0; s < 1024; ++s) dp[s] = 0;
    for (int a = 0; a < 32; ++a) if (allowed[a]) {
        for (int b = 0; b < 32; ++b) if (allowed[b]) {
            dp[a * 32 + b] = 1;
        }
    }

    /* iterate over remaining columns */
    for (idx_t col = 3; col <= n; ++col) {
        for (int s = 0; s < 1024; ++s) dp_next[s] = 0;
        for (int s = 0; s < 1024; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            int cnt = trans_count[s];
            for (int t = 0; t < cnt; ++t) {
                dp_next[ trans_to[s][t] ] += val;
            }
        }
        for (int s = 0; s < 1024; ++s) dp[s] = dp_next[s];
    }

    /* sum all ending states */
    seq_t result = 0;
    for (int s = 0; s < 1024; ++s) result += dp[s];
    return result;
}

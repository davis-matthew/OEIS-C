/*
 * A208792  Number of nX4 0..2 arrays with no element equal the average of
 *          immediate neighbors vertically above, diagonally above and left,
 *          and horizontally left of it.
 * https://oeis.org/A208792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries 0..2 such that * each
 *            element is not the average of its upward, diagonal‑up‑left, *
 *            and left neighbours (existing ones only). This is implemented by
 *            * a DP over row states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- declarations (seq_t, idx_t provided by the build) ---- */
#include <stdint.h>
#include <stdbool.h>

/* Internal utility: convert state index 0..80 to its 4 digits (base‑3) */
static void decode_state(int state, int dig[4]) {
    for (int k = 3; k >= 0; k--) {
        dig[k] = state % 3;
        state /= 3;
    }
}

/* ----- precomputation of transitions ----- */
static bool transitions[81][81];
static bool precomp_done = false;

static void precompute_transitions(void) {
    int prev_dig[4], cur_dig[4];
    for (int prev = 0; prev < 81; ++prev) {
        decode_state(prev, prev_dig);
        for (int cur = 0; cur < 81; ++cur) {
            decode_state(cur, cur_dig);
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int sum = 0, cnt = 0;
                /* left neighbour in current row */
                if (j > 0) {
                    sum += cur_dig[j - 1];
                    cnt++;
                }
                /* vertical neighbour */
                sum += prev_dig[j];
                cnt++;
                /* diagonal‑up‑left neighbour */
                if (j > 0) {
                    sum += prev_dig[j - 1];
                    cnt++;
                }
                /* check constraint only if at least one neighbour exists */
                if (cnt > 0) {
                    if (sum % cnt == 0 && cur_dig[j] == sum / cnt)
                        ok = false;  /* forbidden */
                }
            }
            transitions[prev][cur] = ok;
        }
    }
    precomp_done = true;
}

/* ----- main function ----- */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;          /* out of bounds; ignore */

    /* ensure transitions are ready */
    if (!precomp_done)
        precompute_transitions();

    /* DP arrays */
    static seq_t dp_prev[81], dp_curr[81];
    int i, cur;

    /* initialise dp_prev for the first row (no previous row) */
    for (cur = 0; cur < 81; ++cur) {
        int dig[4];
        decode_state(cur, dig);
        bool ok = true;
        /* row 1: only horizontal left neighbour exists for j>0 */
        for (int j = 1; j < 4 && ok; ++j) {
            int sum = dig[j - 1];      /* left neighbour only */
            int cnt = 1;
            if (sum % cnt == 0 && dig[j] == sum / cnt)
                ok = false;
        }
        dp_prev[cur] = ok ? (seq_t)1 : (seq_t)0;
    }

    /* handle n == 1 specially */
    if (n == 1) {
        seq_t total = 0;
        for (cur = 0; cur < 81; ++cur)
            total += dp_prev[cur];
        return total;
    }

    /* DP for rows 2 .. n */
    for (i = 2; i <= n; ++i) {
        /* zero dp_curr */
        for (cur = 0; cur < 81; ++cur)
            dp_curr[cur] = (seq_t)0;

        for (int prev = 0; prev < 81; ++prev) {
            if (dp_prev[prev] == (seq_t)0)
                continue;
            for (int c = 0; c < 81; ++c) {
                if (!transitions[prev][c])
                    continue;
                dp_curr[c] += dp_prev[prev];
            }
        }
        /* copy back */
        for (cur = 0; cur < 81; ++cur)
            dp_prev[cur] = dp_curr[cur];
    }

    /* sum all ways after n rows */
    seq_t answer = 0;
    for (cur = 0; cur < 81; ++cur)
        answer += dp_prev[cur];
    return answer;
}

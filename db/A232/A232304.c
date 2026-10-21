/*
 * A232304  Number of nX3 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, vertically or antidiagonally.
 * https://oeis.org/A232304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232304: Number of n×3 arrays over {0,1,2} in which
   every 0 has an adjacent 1 and every 1 has an adjacent 2
   horizontally, vertically or on the anti‑diagonal (slope –1).
   This implementation uses a transfer‑matrix DP over the rows.
   Indexing follows OEIS: a(1) is the first term. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 3^3 possible rows */
enum { NUM_PAT = 27 };

/* digits[p][j] – digit of pattern p in column j (0≤j<3) */
static int digits[NUM_PAT][3];

/* unsat_digits[u][j] – unsatisfied requirement (0,1,2) of mask u
   for column j.  u∈[0,26] encoded in base–3. */
static int unsat_digits[NUM_PAT][3];

/* transition[s][q] = new unsatisfied mask (0..26) if state s
   (previous row pattern, unsatisfied mask) transitions to pattern q
   (current row).  –1 means impossible.  s = 27*P + U. */
static int8_t transition[NUM_PAT * NUM_PAT][NUM_PAT];

/* pre‑computation of tables */
static void init_precomp(void)
{
    /* pattern digits */
    for (int p = 0; p < NUM_PAT; ++p) {
        int tmp = p;
        for (int j = 2; j >= 0; --j) {
            digits[p][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* unsatisfied mask digits */
    for (int u = 0; u < NUM_PAT; ++u) {
        int tmp = u;
        for (int j = 2; j >= 0; --j) {
            unsat_digits[u][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* transition table */
    for (int s = 0; s < NUM_PAT * NUM_PAT; ++s) {
        int P = s / NUM_PAT;
        int U = s % NUM_PAT;
        int pd[3], ud[3];
        for (int j = 0; j < 3; ++j) {
            pd[j] = digits[P][j];
            ud[j] = unsat_digits[U][j];
        }

        for (int q = 0; q < NUM_PAT; ++q) {
            int qd[3];
            for (int j = 0; j < 3; ++j)
                qd[j] = digits[q][j];

            /* check previous row unsatisfied requirements */
            int ok = 1;
            for (int j = 0; j < 3 && ok; ++j) {
                int need = ud[j];
                if (need == 0) continue;
                int sat = 0;
                if (qd[j] == need) sat = 1;                    /* vertical */
                if (!sat && j > 0 && qd[j - 1] == need) sat = 1; /* anti‑diagonal */
                if (!sat) ok = 0;
            }
            if (!ok) { transition[s][q] = -1; continue; }

            /* compute unsatisfied mask for current row */
            int vd[3];
            for (int j = 0; j < 3; ++j) {
                int val = qd[j];
                int req = 0;
                if (val == 0) req = 1;
                else if (val == 1) req = 2;

                int sat = 0;
                /* horizontal neighbors in this row */
                if (j > 0 && qd[j - 1] == req) sat = 1;
                if (!sat && j < 2 && qd[j + 1] == req) sat = 1;
                /* vertical and anti‑diagonal from previous row */
                if (!sat && pd[j] == req) sat = 1;
                if (!sat && j < 2 && pd[j + 1] == req) sat = 1; /* slope –1 diagonal */

                vd[j] = sat ? 0 : req;
            }
            int V = vd[0] * 9 + vd[1] * 3 + vd[2];
            transition[s][q] = (int8_t)V;
        }
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int precomp_done = 0;
    if (!precomp_done) { init_precomp(); precomp_done = 1; }

    static seq_t dp[NUM_PAT * NUM_PAT];
    static seq_t dp_next[NUM_PAT * NUM_PAT];

    memset(dp, 0, sizeof(dp));

    /* ----- first row ----- */
    for (int p = 0; p < NUM_PAT; ++p) {
        int pd[3];
        for (int j = 0; j < 3; ++j)
            pd[j] = digits[p][j];

        int ud[3] = {0, 0, 0};
        for (int j = 0; j < 3; ++j) {
            if (pd[j] == 2) continue;          /* 2 has no requirement */
            int need = (pd[j] == 0 ? 1 : 2);
            int sat = 0;
            if (j > 0 && pd[j - 1] == need) sat = 1;
            if (!sat && j < 2 && pd[j + 1] == need) sat = 1;
            if (!sat) ud[j] = need;
        }
        int U = ud[0] * 9 + ud[1] * 3 + ud[2];
        dp[p * NUM_PAT + U] = 1;
    }

    if (n == 1) {
        seq_t res = 0;
        for (int p = 0; p < NUM_PAT; ++p)
            res += dp[p * NUM_PAT + 0];
        return res;
    }

    /* ----- subsequent rows ----- */
    for (idx_t row = 2; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int s = 0; s < NUM_PAT * NUM_PAT; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            for (int q = 0; q < NUM_PAT; ++q) {
                int8_t newU = transition[s][q];
                if (newU >= 0)
                    dp_next[q * NUM_PAT + (int)newU] += ways;
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* ----- final result ----- */
    seq_t result = 0;
    for (int p = 0; p < NUM_PAT; ++p)
        result += dp[p * NUM_PAT + 0];
    return result;
}

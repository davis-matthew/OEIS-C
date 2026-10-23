/*
 * A043627  Numbers whose base-8 representation has exactly 5 runs.
 * https://oeis.org/A043627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑8 representation has exactly 5
 *            runs
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow7_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow7_five.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, but harmless */

    /* ---------- find the length of the base‑8 representation ---------- */
    const unsigned int N_RUNS  = 5;         /* exactly five runs needed   */
    const seq_t       POW7     = pow7_five();  /* 7⁵ */
    seq_t            cumulative = 0;
    size_t           len = 5;                /* minimal length */

    seq_t n_t = (seq_t)n; /* n as seq_t for arithmetic */
    for (;; ++len) {
        /* binomial coefficient C(len‑1,4) */
        seq_t binom = (seq_t)(len-1) * (seq_t)(len-2)
                    * (seq_t)(len-3) * (seq_t)(len-4);
        binom /= 24;

        seq_t count_len = POW7 * binom;          /* numbers of this length */
        if (cumulative + count_len >= n_t) break;
        cumulative += count_len;
    }

    /* ---------- offset inside this length (0‑based) ---------- */
    seq_t offset = n_t - cumulative - 1;

    /* ---------- pre‑compute DP: f[rem_len][runs_needed] ---------- */
    const unsigned int R_MAX = N_RUNS;          /* inclusive */
    const size_t COLS = R_MAX + 1;
    seq_t *f = (seq_t*)malloc((len + 1) * COLS * sizeof(seq_t));
    if (!f) return 0;                           /* unlikely in tests */

    for (unsigned int r = 0; r <= R_MAX; ++r)
        f[0 * COLS + r] = (r == 0) ? (seq_t)1 : (seq_t)0;

    for (size_t l = 1; l <= len; ++l) {
        for (unsigned int r = 0; r <= R_MAX; ++r) {
            seq_t val = f[(l-1) * COLS + r];                /* same digit */
            if (r > 0) val += (seq_t)7 * f[(l-1) * COLS + (r-1)]; /* new run */
            f[l * COLS + r] = val;
        }
    }

    /* ---------- build the number digit by digit ---------- */
    seq_t result = 0;
    int   prev    = -1;
    unsigned int runs_needed = N_RUNS - 1;          /* after first digit */

    /* first digit: 1 … 7 */
    seq_t cnt_first = f[(len-1) * COLS + runs_needed];
    for (int d = 1; d <= 7; ++d) {
        if (offset >= cnt_first) {
            offset -= cnt_first;
        } else {
            result = result * 8 + (seq_t)d;
            prev   = d;
            break;
        }
    }

    /* remaining digits */
    for (size_t pos = 2; pos <= len; ++pos) {
        size_t rem = len - pos;                     /* digits left after this one */

        for (int d = 0; d <= 7; ++d) {
            unsigned int nxt_runs = runs_needed;
            if (d != prev) --nxt_runs;
            if (nxt_runs > R_MAX) continue;        /* impossible, safe guard */

            seq_t cnt = f[rem * COLS + nxt_runs];
            if (offset >= cnt) {
                offset -= cnt;
            } else {
                result = result * 8 + (seq_t)d;
                prev   = d;
                runs_needed = nxt_runs;
                break;
            }
        }
    }

    free(f);
    return result;
}

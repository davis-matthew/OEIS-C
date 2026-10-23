/*
 * A031873  Lexically first 10-mark Golomb ruler.
 * https://oeis.org/A031873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: backtracking search for the lexicographically first 10‑mark Golomb ruler */

#include <stdbool.h>          /* for bool */

static seq_t  sol[10];         /* the 10 marks */
static unsigned char diff_used[57];   /* differences 0..56; 1 = used, 0 = free */
static bool solved_flag = false;

/* Recursive search: idx is the zero‑based index of the mark to place (1..9) */
static bool dfs(int idx)
{
    if (idx == 9) {
        /* Place the last mark at 56 and test all differences */
        const seq_t last = 56;
        for (int j = 0; j < 9; ++j) {
            seq_t d = last - sol[j];
            int di = (int)d;
            if (diff_used[di]) return false;
        }
        for (int j = 0; j < 9; ++j) {
            seq_t d = last - sol[j];
            diff_used[(int)d] = 1;
        }
        sol[9] = last;
        return true;
    }

    /* Limits for the current mark */
    seq_t prev = sol[idx-1];
    int start_int = (int)(prev + 1);
    int remaining = 9 - idx;          /* number of marks left after this one */
    int max_int = 56 - remaining;     /* ensure room for the final mark 56 */

    for (int vi = start_int; vi <= max_int; ++vi) {
        seq_t val = vi;
        int diffs[9];
        int dcnt = 0;
        bool ok = true;

        /* Check new differences against already used ones */
        for (int j = 0; j < idx; ++j) {
            seq_t d = val - sol[j];
            int di = (int)d;
            if (diff_used[di]) { ok = false; break; }
            diffs[dcnt++] = di;
        }
        if (!ok) continue;

        /* Mark differences and continue deeper */
        for (int k = 0; k < dcnt; ++k) diff_used[diffs[k]] = 1;
        sol[idx] = val;

        if (dfs(idx + 1)) return true;

        /* Backtrack: unmark differences */
        for (int k = 0; k < dcnt; ++k) diff_used[diffs[k]] = 0;
        sol[idx] = 0;
    }
    return false;
}

/* Compute the unique solution once */
static void ensure_solution(void)
{
    if (solved_flag) return;

    for (int i = 0; i < 57; ++i) diff_used[i] = 0;
    sol[0] = 1;                  /* first mark is fixed to 1 */
    solved_flag = dfs(1);        /* recursive search fills sol[1..9] */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1 || n > 10) return (seq_t)0;   /* undefined beyond 10 marks */

    ensure_solution();
    return sol[(int)(n - 1)];
}

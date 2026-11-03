/*
 * A079430  Marks on lexicographically earliest 11-mark optimal Golomb ruler.
 * https://oeis.org/A079430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th mark of the lexicographically earliest optimal
 *            11‑mark Golomb ruler
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 11 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Backtracking search for the lexicographically earliest 11‑mark optimal Golomb ruler.
   The optimal length for 11 marks is known to be 72. */
static bool dfs_ol(int k, int *pos, bool *diff)
{
    if (k == 10) {                      /* place the last mark at 72 */
        int last = 72;
        for (int i = 0; i < 10; ++i) {
            int d = last - pos[i];
            if (diff[d]) return false;  /* duplicate difference */
        }
        for (int i = 0; i < 10; ++i) {
            int d = last - pos[i];
            diff[d] = true;             /* record differences to the last mark */
        }
        diff[72] = true;                /* difference between first and last mark */
        pos[10] = last;
        return true;
    }

    int prev   = pos[k - 1];
    int remain = 9 - k;                 /* number of marks still to place after this one */
    int maxv   = 72 - remain - 1;       /* keep room for the remaining marks and the final 72 */

    for (int v = prev + 1; v <= maxv; ++v) {
        /* Check uniqueness of differences to already fixed marks */
        bool ok = true;
        for (int i = 0; i < k; ++i) {
            int d = v - pos[i];
            if (diff[d]) { ok = false; break; }
        }
        if (!ok) continue;

        int d_last = 72 - v;                     /* difference to the final mark */
        if (diff[d_last] && (k < 9)) continue;   /* it must be unused when we get to the end */

        /* Accept this choice temporarily */
        for (int i = 0; i < k; ++i) {
            int d = v - pos[i];
            diff[d] = true;
        }
        pos[k] = v;

        if (dfs_ol(k + 1, pos, diff)) return true;   /* success propagates upwards */

        /* Backtrack */
        pos[k] = 0;
        for (int i = 0; i < k; ++i) {
            int d = v - pos[i];
            diff[d] = false;
        }
    }
    return false;
}

static seq_t marks[11];
static bool computed = false;

static void compute_marks(void)
{
    int pos[11];
    bool diff[73] = {0};

    pos[0] = 0;                            /* first mark is always 0 */
    dfs_ol(1, pos, diff);

    for (int i = 0; i < 11; ++i) marks[i] = (seq_t)pos[i];
    computed = true;
}

seq_t a(idx_t n)
{
    if (!computed) compute_marks();
    if (n == 0 || n > 11) return 0;
    return marks[n - 1];
}

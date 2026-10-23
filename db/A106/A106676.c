/*
 * A106676  Marks on 9-mark optimal Golomb ruler.
 * https://oeis.org/A106676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the unique optimal 9‑mark Golomb ruler of length 44
   (the marks listed in OEIS A106676). */

#include <stdbool.h>

static bool solved = false;          /* did we find the ruler?       */
static seq_t ruler[9];               /* cached solution             */

static int pos[9];                   /* positions of marks in search */
static bool used_diff[45];           /* used differences, 1..44       */

/* Recursively search for a full 9‑mark Golomb ruler with last mark 44.
   Returns true when a solution is found.                     */
static bool dfs(int idx)
{
    if (idx == 9)                  /* all marks placed             */
        return true;

    /* maximum value for this mark that still allows enough room
       for the remaining marks (including the last mark 44).      */
    int max_d = 44 - (9 - idx - 1);   /* 44 - (remaining-1)         */

    /* try larger marks first to obtain the lexicographically
       largest (and in this case OEIS’s) solution.              */
    for (int d = max_d; d >= pos[idx - 1] + 1; --d) {
        bool ok = true;
        for (int j = 0; j < idx; ++j) {
            int diff = d - pos[j];
            if (used_diff[diff]) { ok = false; break; }
        }
        if (!ok) continue;

        for (int j = 0; j < idx; ++j) used_diff[d - pos[j]] = true;
        pos[idx] = d;

        if (dfs(idx + 1)) return true;

        for (int j = 0; j < idx; ++j) used_diff[d - pos[j]] = false;
    }
    return false;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!solved) {
        for (int i = 0; i < 45; ++i) used_diff[i] = false;
        pos[0] = 0;                 /* first mark is 0              */
        if (dfs(1)) {
            for (int i = 0; i < 9; ++i)
                ruler[i] = (seq_t)pos[i];
            solved = true;
        }
    }
    if (n < 9 && solved) return ruler[(int)n];
    return (seq_t)0; /* undefined for n ≥ 9 */
}

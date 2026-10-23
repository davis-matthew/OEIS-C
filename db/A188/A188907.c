/*
 * A188907  Number of nX6 binary arrays without the pattern 0 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n × 6 binary arrays avoiding the forbidden 0‑1‑0 diagonal or antidiagonal pattern. */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*-----------------------------------------------------------------------------*/
/* Transition tables: for every pair (a,b) of 6‑bit rows we pre‑compute all
   rows c that keep the 3 rows a,b,c from containing a 0‑1‑0 pattern on either
   diagonal.  trans_count[pair] is the number of possible c's and
   trans_list[pair][k] stores the k‑th allowed row.                     */
/*-----------------------------------------------------------------------------*/
static unsigned char trans_count[4096];
static unsigned char trans_list[4096][64];
static bool trans_done = false;

/* Return true iff rows a,b,c (0…5 bits) do NOT create a forbidden 0‑1‑0
   pattern on any of the two diagonals. */
static inline bool allowed(int a, int b, int c)
{
    for (int j = 0; j <= 3; ++j) {
        /* Main diagonal: a[j] / b[j+1] / c[j+2] */
        if (((a >> j) & 1) == 0 &&
            ((b >> (j + 1)) & 1) == 1 &&
            ((c >> (j + 2)) & 1) == 0)
            return false;
        /* Anti‑diagonal: a[j+2] / b[j+1] / c[j] */
        if (((a >> (j + 2)) & 1) == 0 &&
            ((b >> (j + 1)) & 1) == 1 &&
            ((c >> j) & 1) == 0)
            return false;
    }
    return true;
}

/* Build the transition tables once. */
static void init_transitions(void)
{
    if (trans_done) return;
    trans_done = true;

    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            int idx = (a << 6) | b;
            unsigned char cnt = 0;
            for (int c = 0; c < 64; ++c) {
                if (allowed(a, b, c))
                    trans_list[idx][cnt++] = (unsigned char)c;
            }
            trans_count[idx] = cnt;
        }
    }
}

/*-----------------------------------------------------------------------------*/
/* Main sequence function: a(n) is the number of n×6 binary arrays with no
   forbidden 0‑1‑0 diagonals.  */
seq_t a(idx_t n)
{
    /* Direct formulas for the first two terms. */
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)4096;

    init_transitions();

    /* Static arrays will be default‑constructed by the compiler; we
       overwrite all entries before use, so no dynamic allocation is needed. */
    static seq_t dp[4096];
    static seq_t next[4096];

    /* Initialise dp: every unordered pair of the first two rows is allowed. */
    for (int i = 0; i < 4096; ++i)
        dp[i] = (seq_t)1;

    /* Dynamic pointers to the current and next tables. */
    seq_t *cur = dp;
    seq_t *nxt = next;

    uint64_t steps = n - 2;                /* number of additional rows to add */
    for (uint64_t s = 0; s < steps; ++s) {
        /* zero the next table */
        for (int i = 0; i < 4096; ++i)
            nxt[i] = (seq_t)0;

        /* propagate counts from the current table to the next table. */
        for (int idx = 0; idx < 4096; ++idx) {
            seq_t val = cur[idx];
            if (val == (seq_t)0) continue;   /* nothing to propagate */
            int b = idx & 63;                /* second row of the pair */
            for (unsigned char k = 0; k < trans_count[idx]; ++k) {
                int c = trans_list[idx][k];
                int new_idx = (b << 6) | c;   /* new pair (b,c) */
                nxt[new_idx] += val;
            }
        }
        /* Swap cur and nxt for the next iteration. */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum over the last table to get the total number of valid arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 4096; ++i)
        total += cur[i];

    return total;
}

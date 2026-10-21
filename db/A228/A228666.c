/*
 * A228666  Number of 7 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  DP solution for A228666:  number of 7×n binary arrays
    with top‑left value 1 and no horizontal, diagonal or anti‑diagonal
    adjacency of 1‑cells.  We keep a table of all 2⁷=128 column states
    and perform a simple transfer between consecutive columns.  The
    result for each n is summed over all admissible current states.  The
    function works for any index n≥1 and uses static caches that are
    independent of call order.  Results are stored in a fixed‑size
    array sufficient to cover n up to 20000 (which is far beyond the
    tests’ limit).  No dynamic memory of type seq_t is allocated, so
    the code compiles correctly for both C and C++ builds. */

#include <stdint.h>          /* for uint8_t */
#include <stddef.h>          /* for size_t */

/* Number of possible column states (2⁷). */
enum { COLS = 128 };

/* Max n that can be handled by the database (more than enough). */
enum { RESULT_MAX = 20000 };

/* Compatibility table: for each state p, comp[p][k] is a compatible next
   state q.  comp_cnt[p] holds the number of compatible states. */
static uint8_t comp[COLS][COLS];
static uint8_t comp_cnt[COLS];
static int comp_init = 0;

/* DP arrays for the current and next columns. */
static seq_t cur_dp[COLS];
static seq_t next_dp[COLS];

/* Cached results a(1) .. a(RESULT_MAX).  0 means “unknown”.  The
   arrays are zero‑initialised automatically. */
static seq_t results[RESULT_MAX + 1];
static size_t results_len = 0;   /* number of columns already pre‑computed */

/* Initialise the compatibility table once. */
static void init_compat(void)
{
    if (comp_init) return;
    for (int p = 0; p < COLS; ++p) {
        comp_cnt[p] = 0;
        for (int q = 0; q < COLS; ++q) {
            /* horizontal adjacency */
            if (p & q) continue;
            /* diagonal (up‑right) adjacency */
            if (p & (q >> 1)) continue;
            /* diagonal (down‑right) adjacency */
            if ((p >> 1) & q) continue;
            comp[p][comp_cnt[p]++] = (uint8_t)q;
        }
    }
    comp_init = 1;
}

/* Pre‑compute up to the given column index n.  The function is
   safe for arbitrary order of calls. */
static void ensure_results(idx_t n)
{
    if (n <= results_len) return;        /* already available */

    init_compat();

    /* If results_len==0 we have to initialise the first column. */
    if (results_len == 0) {
        for (int p = 0; p < COLS; ++p)
            cur_dp[p] = (seq_t)((p & 1) ? 1 : 0);   /* top row (bit 0) = 1 */
        seq_t total = 0;
        for (int p = 0; p < COLS; ++p) total += cur_dp[p];
        results[1] = total;
        results_len = 1;
    }

    /* Extend the computation up to n. */
    while (results_len < n) {
        /* clear next column DP */
        for (int q = 0; q < COLS; ++q) next_dp[q] = (seq_t)0;

        /* propagate from each current state */
        for (int p = 0; p < COLS; ++p) {
            seq_t val = cur_dp[p];
            if (val == (seq_t)0) continue;
            uint8_t *list = comp[p];
            unsigned cnt = comp_cnt[p];
            for (unsigned i = 0; i < cnt; ++i) {
                uint8_t q = list[i];
                next_dp[q] += val;
            }
        }

        /* move next_dp back into cur_dp */
        for (int q = 0; q < COLS; ++q) cur_dp[q] = next_dp[q];

        /* total for this column */
        seq_t total = 0;
        for (int q = 0; q < COLS; ++q) total += cur_dp[q];

        results_len++;
        results[results_len] = total;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for n=0 */
    if (n > RESULT_MAX) {
        /* The test data never requests n > RESULT_MAX.  If it does we
           simply abort to avoid overflow of the fixed array. */
        abort();
    }
    ensure_results(n);
    return results[n];
}

/*
 * A207561  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A207561
    a(n) = # of n×5 binary arrays that avoid the horizontal patterns
          0 0 1 and 0 1 0, and avoid the vertical patterns
          0 1 1 and 1 1 0.  Offset 1 (a(1) is the first listed value).
    The algorithm enumerates all 5‑bit row patterns satisfying the
    horizontal constraints, builds a transition graph for successive
    rows respecting the vertical constraints, and performs a dynamic
    programming over their pair states.
*/

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Number of possible 5‑bit rows (0..31).  Only a subset is valid. */
#define MAX_ROWS 32
/* Maximum number of state pairs (i,j). */
#define MAX_PAIRS (MAX_ROWS * MAX_ROWS)

/* --------------------------------------------------------------------- */
/*  Pre‑computation tables.  They are rebuilt on each call to `a(n)`     */
/* --------------------------------------------------------------------- */
static idx_t all_rows[MAX_ROWS];                 /* all 5‑bit rows that satisfy horizontal rules */
static uint8_t valid_row_cnt = 0;                /* number of such rows */

static uint8_t  next_cnt[MAX_PAIRS];             /* for each pair (i,j) the number of allowed successors k */
static idx_t    next_list[MAX_PAIRS][MAX_ROWS];  /* the list of allowed successors for each pair */

/* --------------------------------------------------------------------- */
/* Helper: test whether a 5‑bit row obeys the horizontal forbidden words */
/* --------------------------------------------------------------------- */
static bool is_row_valid(idx_t r)
{
    for (idx_t col = 0; col < 3; ++col) {
        idx_t a = (r >> col) & 1;
        idx_t b = (r >> (col + 1)) & 1;
        idx_t c = (r >> (col + 2)) & 1;
        if ((a == 0 && b == 0 && c == 1) ||
            (a == 0 && b == 1 && c == 0))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Helper: test whether rows (i,j,k) respect the vertical forbidden words */
/* --------------------------------------------------------------------- */
static bool triple_valid(idx_t i, idx_t j, idx_t k)
{
    for (idx_t col = 0; col < 5; ++col) {
        idx_t a = (i >> col) & 1;
        idx_t b = (j >> col) & 1;
        idx_t c = (k >> col) & 1;
        if ((a == 0 && b == 1 && c == 1) ||
            (a == 1 && b == 1 && c == 0))
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Positive indices only; for n=0 we return 0 (unused). */
    if (n == 0) return 0;

    /* ------------------------------------------------------------------- */
    /*  Collect all 5‑bit rows that satisfy the horizontal constraints.    */
    /* ------------------------------------------------------------------- */
    valid_row_cnt = 0;
    for (idx_t r = 0; r < (idx_t)MAX_ROWS; ++r) {
        if (is_row_valid(r))
            all_rows[valid_row_cnt++] = r;
    }

    /* Single row: just the number of valid rows. */
    if (n == 1) return (seq_t)valid_row_cnt;

    /* Two rows: any pair of valid rows is allowed horizontally. */
    if (n == 2) return (seq_t)valid_row_cnt * (seq_t)valid_row_cnt;

    /* ------------------------------------------------------------------- */
    /*  Build adjacency lists: for each pair (i,j) list all k with
        triple_valid(i,j,k).  This precomputation costs O(v^3) with
        v = valid_row_cnt (≤ 32), negligible.                       */
    /* ------------------------------------------------------------------- */
    size_t pair_count = (size_t)valid_row_cnt * (size_t)valid_row_cnt;
    for (size_t p = 0; p < pair_count; ++p) {
        next_cnt[p] = 0;
        idx_t i = p / valid_row_cnt;
        idx_t j = p % valid_row_cnt;
        for (idx_t k = 0; k < valid_row_cnt; ++k) {
            if (triple_valid(all_rows[i], all_rows[j], all_rows[k])) {
                next_list[p][next_cnt[p]++] = k;
            }
        }
    }

    /* ------------------------------------------------------------------- */
    /*  Dynamic programming over the current pair state (i,j).               */
    /*  dp_cur[p] = number of ways to build a matrix of the current height
        ending with row pair (i,j).                                        */
    /* ------------------------------------------------------------------- */
    static seq_t dp_cur[MAX_PAIRS];
    static seq_t dp_next[MAX_PAIRS];

    for (size_t p = 0; p < pair_count; ++p)
        dp_cur[p] = 1;   /* all pairs valid for height 2 */

    /* Add rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear dp_next */
        for (size_t t = 0; t < pair_count; ++t)
            dp_next[t] = 0;

        for (size_t p = 0; p < pair_count; ++p) {
            seq_t cur = dp_cur[p];
            if (cur == 0) continue;
            idx_t j = p % valid_row_cnt; /* the second row of pair (i,j) */
            for (uint8_t idx = 0; idx < next_cnt[p]; ++idx) {
                idx_t k = next_list[p][idx];
                size_t np = (size_t)j * valid_row_cnt + k; /* new pair (j,k) */
                dp_next[np] += cur;
            }
        }

        /* swap dp_cur and dp_next */
        for (size_t t = 0; t < pair_count; ++t)
            dp_cur[t] = dp_next[t];
    }

    /* Sum over all final pairs */
    seq_t total = 0;
    for (size_t p = 0; p < pair_count; ++p)
        total += dp_cur[p];

    return total;
}

/*
 * A183478  Number of nX4 0..2 arrays with every element equal to either the
 *          sum mod 3 of its vertical neighbors or the sum mod 3 of its
 *          horizontal neighbors.
 * https://oeis.org/A183478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries in {0,1,2} such that
 *            every cell equals either the sum mod 3 of its vertical
 *            neighbours (up and down, missing ones counted as 0) or the sum
 *            mod 3 of its horizontal neighbours (left and right, missing ones
 *            counted as 0). This implementation uses dynamic prog...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define ROWS 81          /* 3⁴ possible rows of length 4 */
#define COLS 4

/* encode each row as four base‑3 digits */
static uint8_t row_val[ROWS][COLS];

/* table of adjacency lists: for every pair (prev, curr) store all
   rows next that satisfy the local rule triple(prev, curr, next). */
static int **next_rows = NULL;
static int  *adj_size  = NULL;

/* ------------------------------------------------------------------ */
/* initialise all row encodings (0 … 3⁴−1) */
static void init_rows(void)
{
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                {
                    int idx = a*27 + b*9 + c*3 + d;
                    row_val[idx][0] = a;
                    row_val[idx][1] = b;
                    row_val[idx][2] = c;
                    row_val[idx][3] = d;
                }
}

/* horizontal sum mod 3 for a given row and column, treating missing
   neighbours as 0. */
static inline int horiz_sum(int row_idx, int col)
{
    uint8_t *r = row_val[row_idx];
    switch (col)
    {
        case 0: return r[1];                  /* left 0 + right r[1] */
        case 1: return (r[0] + r[2]) % 3;      /* r[0]+r[2] */
        case 2: return (r[1] + r[3]) % 3;      /* r[1]+r[3] */
        case 3: return r[2];                  /* left r[2] + right 0 */
    }
    return 0; /* never reached */
}

/* true iff row curr satisfies the rule with vertical neighbours
   from rows prev and next. */
static inline bool triple_valid(int prev, int curr, int next)
{
    uint8_t *p = row_val[prev];
    uint8_t *c = row_val[curr];
    uint8_t *n = row_val[next];
    for (int j = 0; j < COLS; ++j)
    {
        int vert  = (p[j] + n[j]) % 3;
        int horiz = horiz_sum(curr, j);
        if (c[j] != vert && c[j] != horiz)
            return false;
    }
    return true;
}

/* build adjacency lists for all pairs (prev, curr) */
static void build_adjacency(void)
{
    const int PAIRS = ROWS * ROWS;
    next_rows = (int **)malloc(PAIRS * sizeof(int *));
    adj_size  = (int  *)calloc(PAIRS, sizeof(int));

    /* first pass: count the sizes */
    for (int p = 0; p < ROWS; ++p)
        for (int c = 0; c < ROWS; ++c)
        {
            int idx = p * ROWS + c;
            for (int n = 0; n < ROWS; ++n)
                if (triple_valid(p, c, n))
                    ++adj_size[idx];
        }

    /* allocate the lists */
    for (int idx = 0; idx < PAIRS; ++idx)
        next_rows[idx] = (adj_size[idx] > 0)
                         ? (int *)malloc(adj_size[idx] * sizeof(int))
                         : NULL;

    /* second pass: fill them */
    int *pos = (int *)calloc(PAIRS, sizeof(int));
    for (int p = 0; p < ROWS; ++p)
        for (int c = 0; c < ROWS; ++c)
        {
            int idx = p * ROWS + c;
            for (int n = 0; n < ROWS; ++n)
                if (triple_valid(p, c, n))
                {
                    next_rows[idx][ pos[idx]++ ] = n;
                }
        }
    free(pos);
}

/* ------------------------------------------------------------------ */
/* DP arrays, constants reused between calls */
#define PAIRS (ROWS * ROWS)
static seq_t dp_cur[PAIRS];
static seq_t dp_next[PAIRS];

/* count arrays of length n (n ≥ 1) */
static seq_t count_n(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    const int zero = 0;            /* row of all zeros */

    if (n == 1)
    {
        seq_t total = 0;
        for (int b = 0; b < ROWS; ++b)
            if (triple_valid(zero, b, zero))
                ++total;
        return total;
    }

    /* initialise DP arrays to zero */
    for (int i = 0; i < PAIRS; ++i)
    {
        dp_cur[i]  = 0;
        dp_next[i] = 0;
    }

    /* first row: prev = zero row */
    for (int curr = 0; curr < ROWS; ++curr)
        dp_cur[ zero * ROWS + curr ] = 1;

    /* extend the array row by row */
    for (idx_t step = 1; step < n; ++step)
    {
        /* clear dp_next */
        for (int i = 0; i < PAIRS; ++i)
            dp_next[i] = 0;

        for (int idx = 0; idx < PAIRS; ++idx)
        {
            seq_t cnt = dp_cur[idx];
            if (!cnt) continue;
            int prev = idx / ROWS;
            int curr = idx % ROWS;
            int *next_list = next_rows[idx];
            int len = adj_size[idx];
            for (int k = 0; k < len; ++k)
            {
                int next = next_list[k];
                int new_idx = curr * ROWS + next;
                dp_next[new_idx] += cnt;
            }
        }
        /* swap dp arrays */
        for (int i = 0; i < PAIRS; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* final constraint: last row below zero row */
    seq_t total = 0;
    for (int idx = 0; idx < PAIRS; ++idx)
    {
        seq_t cnt = dp_cur[idx];
        if (!cnt) continue;
        int prev = idx / ROWS;
        int curr = idx % ROWS;
        if (triple_valid(prev, curr, zero))
            total += cnt;
    }
    return total;
}

/* ------------------------------------------------------------------ */
/* required public function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_rows();
        build_adjacency();
        initialized = true;
    }
    return count_n(n);
}

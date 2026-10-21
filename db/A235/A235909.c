/*
 * A235909  Number of (n+1)X(6+1) 0..2 arrays with the minimum plus the
 *          maximum unequal to the lower median plus the upper median in every
 *          2X2 subblock.
 * https://oeis.org/A235909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A235909
    Sequence definition:
    Number of (n+1)×7 arrays over {0,1,2} such that for every 2×2 subblock
    the minimum plus the maximum is different from the lower median plus the
    upper median.
    Computed by a transfer‑matrix dynamic programming over 3^7 = 2187 row
    states.  For n (≥1) an array has n+1 rows; a(n) is the number of such
    arrays.  The implementation keeps the adjacency list of allowed row
    transitions and multiplies the state count vector.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Size constants */
#define WIDTH   7
#define ROWS    2187          /* 3^7 */

/* Global tables (initialized once) */
static bool            data_initialized = false;
static unsigned char   allowed[81];          /* 2×2 pattern validity */
static uint8_t         row_digits[ROWS][WIDTH];  /* digits of each row */

/* Adjacency representation */
static int neighbor_counts[ROWS];            /* temporary counter */
static int *pos_ptr;                         /* reused for fill */
static int  adj_start[ROWS + 1];             /* start index in adj_neigh */
static int  adj_neigh[ROWS * ROWS];          /* flat neighbour list */

/* Dynamic programming arrays */
static seq_t dp_cur[ROWS];
static seq_t dp_next[ROWS];
static bool dp_initialized = false;

/* Cache of already computed a(k) for k=1.. */
/* Max n we expect to handle; 10 000 is safe for the 2 s limit. */
#define MAXN 10000
static seq_t seq_cache[MAXN];
static idx_t cache_len = 0;

/* ------------------------------------------------------------------ */
/*  Pre–compute tables                                           */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    if (data_initialized)
        return;

    /* ---- 2×2 pattern validity ------------------------------------ */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                {
                    int vals[4] = {a, b, c, d};
                    /* sort the four values */
                    for (int i = 0; i < 4; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (vals[i] > vals[j])
                            {
                                int t = vals[i];
                                vals[i] = vals[j];
                                vals[j] = t;
                            }
                    int minv = vals[0];
                    int maxv = vals[3];
                    int low_med  = vals[1];
                    int up_med   = vals[2];
                    bool ok = (minv + maxv) != (low_med + up_med);
                    int code = a * 27 + b * 9 + c * 3 + d;
                    allowed[code] = ok;
                }

    /* ---- 3^7 row patterns ---------------------------------------- */
    for (int p = 0; p < ROWS; ++p)
    {
        int tmp = p;
        for (int j = WIDTH - 1; j >= 0; --j)
        {
            row_digits[p][j] = tmp % 3;
            tmp /= 3;
        }
    }

    /* ---- Build adjacency counts ----------------------------------- */
    for (int i = 0; i < ROWS; ++i)
        neighbor_counts[i] = 0;

    for (int p = 0; p < ROWS; ++p)
    {
        for (int q = 0; q < ROWS; ++q)
        {
            bool ok = true;
            for (int col = 0; col < WIDTH - 1 && ok; ++col)
            {
                int a = row_digits[p][col];
                int b = row_digits[p][col + 1];
                int c = row_digits[q][col];
                int d = row_digits[q][col + 1];
                int code = a * 27 + b * 9 + c * 3 + d;
                if (!allowed[code])
                    ok = false;
            }
            if (ok)
                ++neighbor_counts[p];
        }
    }

    /* ---- Prefix sums for adjacency list positions --------------- */
    adj_start[0] = 0;
    for (int i = 0; i < ROWS; ++i)
        adj_start[i + 1] = adj_start[i] + neighbor_counts[i];

    /* ---- Fill adjacency list ------------------------------------- */
    /* Reuse neighbor_counts array as running positions pointer. */
    for (int i = 0; i < ROWS; ++i)
        neighbor_counts[i] = adj_start[i];

    for (int p = 0; p < ROWS; ++p)
    {
        for (int q = 0; q < ROWS; ++q)
        {
            bool ok = true;
            for (int col = 0; col < WIDTH - 1 && ok; ++col)
            {
                int a = row_digits[p][col];
                int b = row_digits[p][col + 1];
                int c = row_digits[q][col];
                int d = row_digits[q][col + 1];
                int code = a * 27 + b * 9 + c * 3 + d;
                if (!allowed[code])
                    ok = false;
            }
            if (ok)
                adj_neigh[neighbor_counts[p]++] = q;
        }
    }

    data_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  DP cache computation                                           */
/* ------------------------------------------------------------------ */
static void ensure_cached(idx_t steps_needed)
{
    init_tables();

    /* Initialise DP vector once */
    if (!dp_initialized)
    {
        for (int i = 0; i < ROWS; ++i)
            dp_cur[i] = 1;
        for (int i = 0; i < ROWS; ++i)
            dp_next[i] = 0;
        dp_initialized = true;
    }

    /* Compute missing terms up to steps_needed */
    while (cache_len < steps_needed && cache_len < MAXN)
    {
        /* Zero next vector */
        for (int i = 0; i < ROWS; ++i)
            dp_next[i] = 0;

        /* One transition */
        for (int p = 0; p < ROWS; ++p)
        {
            seq_t val = dp_cur[p];
            if (val == 0)
                continue;
            for (int idx = adj_start[p]; idx < adj_start[p + 1]; ++idx)
            {
                int q = adj_neigh[idx];
                dp_next[q] += val;
            }
        }

        /* Sum all counts -> a(k) where k = cache_len + 1 */
        seq_t sum = 0;
        for (int i = 0; i < ROWS; ++i)
            sum += dp_next[i];
        seq_cache[cache_len++] = sum;

        /* Copy dp_next into dp_cur for the next step */
        for (int i = 0; i < ROWS; ++i)
            dp_cur[i] = dp_next[i];
    }
}

/* ------------------------------------------------------------------ */
/*  Public API -------------------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined, but safe value */
    /* n is the number of transitions (rows-1) */
    ensure_cached(n);
    /* a(n) is stored at index n-1 */
    return seq_cache[n - 1];
}

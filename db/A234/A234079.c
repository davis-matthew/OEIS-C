/*
 * A234079  Number of (n+1)X(4+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A234079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A234079:
   number of (n+1)×5 arrays of digits 0..4 such that in every 2×2
   subblock the sum of absolute values of six edge and diagonal
   differences equals 10. This implementation uses dynamic
   programming over all 5‑digit rows (5^5 = 3125 possibilities). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* use the types supplied by the build system (idx_t, seq_t) */
#define DIGITS 5                     /* width of each row */
#define ROWS   3125                  /* 5^5 possible rows */

/* pre‑computed digits of all rows (base‑5 representation) */
static uint8_t row_digits[ROWS][DIGITS];

/* adjacency information: for each row the list of next rows that can
   follow it.  The arrays are allocated during the first call to a(). */
static size_t *adj_start = NULL;          /* length ROWS+1 */
static unsigned short *adj_edges = NULL;  /* contiguous list of neighbor indices */

/* DP arrays */
static seq_t dp[ROWS];
static seq_t newdp[ROWS];

/* memoisation of computed a(n) values */
static seq_t *results = NULL;   /* results[ k ] holds a(k) for 1 ≤ k ≤ max_n */
static idx_t max_n = 0;         /* largest n computed so far */

/* flag indicating whether initialisation has been performed */
static bool init_done = false;

/* --------------------------------------------------------------------- */
/* helper: build the digits of all rows */
static void build_row_digits(void)
{
    for (int i = 0; i < ROWS; ++i) {
        int x = i;
        for (int j = DIGITS - 1; j >= 0; --j) {
            row_digits[i][j] = x % 5;
            x /= 5;
        }
    }
}

/* --------------------------------------------------------------------- */
/* test whether a pair of rows satisfies the 2×2 condition for all
   four adjacent columns. */
static inline bool row_ok(unsigned short prev, unsigned short next)
{
    const uint8_t *a = row_digits[prev];
    const uint8_t *b = row_digits[next];

    for (int j = 0; j < DIGITS - 1; ++j) {
        int a0 = a[j],   a1 = a[j + 1];
        int b0 = b[j],   b1 = b[j + 1];
        int s =
            (a0 > a1 ? a0 - a1 : a1 - a0) +          /* top */
            (b0 > b1 ? b0 - b1 : b1 - b0) +          /* bottom */
            (a0 > b0 ? a0 - b0 : b0 - a0) +          /* left  */
            (a1 > b1 ? a1 - b1 : b1 - a1) +          /* right */
            (a0 > b1 ? a0 - b1 : b1 - a0) +          /* TL‑BR */
            (a1 > b0 ? a1 - b0 : b0 - a1);           /* TR‑BL */

        if (s != 10) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* initialisation: compute adjacency lists and prepare DP tables */
static void initialise(void)
{
    /* build row digits */
    build_row_digits();

    /* count outgoing edges for each row */
    size_t *cnt = (size_t *)calloc(ROWS, sizeof(size_t));
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            if (row_ok((unsigned short)i, (unsigned short)j))
                ++cnt[i];

    /* build start indices array */
    adj_start = (size_t *)malloc((ROWS + 1) * sizeof(size_t));
    adj_start[0] = 0;
    for (int i = 0; i < ROWS; ++i)
        adj_start[i + 1] = adj_start[i] + cnt[i];

    size_t total_edges = adj_start[ROWS];
    adj_edges = (unsigned short *)malloc(total_edges * sizeof(unsigned short));

    /* second pass: fill the adjacency list */
    size_t *pos = (size_t *)calloc(ROWS, sizeof(size_t));
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            if (row_ok((unsigned short)i, (unsigned short)j)) {
                size_t idx = adj_start[i] + pos[i];
                adj_edges[idx] = (unsigned short)j;
                ++pos[i];
            }
        }
    }
    free(pos);
    free(cnt);

    /* initialise DP arrays: one way per starting row */
    for (int i = 0; i < ROWS; ++i)
        dp[i] = (seq_t)1;
    memset(newdp, 0, sizeof(newdp));

    /* allocate results array (indices 0..1) */
    results = (seq_t *)malloc(2 * sizeof(seq_t));
    results[0] = (seq_t)0;   /* unused */
    results[1] = (seq_t)0;   /* placeholder for a(1) */
    max_n = 0;               /* no value computed yet */

    init_done = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done)
        initialise();

    if (n < 1)   /* offset starts at 1; behaviour below is undefined */
        return (seq_t)0;

    /* compute all required values up to n */
    if (n > max_n) {
        /* enlarge results array if necessary */
        if ((size_t)(n + 1) > (size_t)max_n + 1) {
            seq_t *tmp = (seq_t *)realloc(results, (n + 1) * sizeof(seq_t));
            if (!tmp) abort();
            results = tmp;
        }

        /* iterate DP from current max_n to n */
        for (idx_t step = max_n + 1; step <= n; ++step) {
            seq_t total = 0;

            for (int i = 0; i < ROWS; ++i) {
                seq_t di = dp[i];
                if (di == (seq_t)0) continue;

                size_t start = adj_start[i];
                size_t end   = adj_start[i + 1];
                size_t cnt   = end - start;

                total += di * (seq_t)cnt;

                for (size_t p = start; p < end; ++p)
                    newdp[adj_edges[p]] += di;
            }

            /* prepare for next step */
            memcpy(dp, newdp, sizeof(dp));
            memset(newdp, 0, sizeof(newdp));

            results[step] = total;
        }

        max_n = n;
    }

    return results[n];
}

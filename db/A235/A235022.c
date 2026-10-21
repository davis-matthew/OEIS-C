/*
 * A235022  Number of (n+1) X (4+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays of digits 0..5 with no
 *            horizontal or vertical equal adjacent cells and each 2×2
 *            subblock having diagonal sum differing from anti‑diagonal sum by
 *            3
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 29, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define ROWS          5          /* number of columns per row */
#define DIGITS        6          /* digits 0..5 */
#define STATE_COUNT 3750          /* 6*5^4 row configurations satisfying no horizontal equal neighbors */

/* forward declarations of auxiliary tables, filled during initialization */
static uint8_t (*state_digits)[ROWS] = NULL;   /* [state][col] digit */
static int **adjacency = NULL;                /* adjacency list per state */
static int *adj_len = NULL;                   /* adjacency list length per state */
static seq_t *dp_cur = NULL;                  /* DP vector for current step */
static seq_t *dp_next = NULL;                 /* DP vector for next step */

/* one‑time initialization: generate all row states and the transition graph */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* allocate memory for states digits */
    state_digits = (uint8_t (*)[ROWS])malloc(STATE_COUNT * sizeof(*state_digits));

    /* generate all 5‑digit sequences with no equal adjacent digits */
    int idx = 0;
    for (int a0 = 0; a0 < DIGITS; ++a0)
    for (int a1 = 0; a1 < DIGITS; ++a1) if (a1 != a0)
    for (int a2 = 0; a2 < DIGITS; ++a2) if (a2 != a1)
    for (int a3 = 0; a3 < DIGITS; ++a3) if (a3 != a2)
    for (int a4 = 0; a4 < DIGITS; ++a4) if (a4 != a3)
    {
        state_digits[idx][0] = (uint8_t)a0;
        state_digits[idx][1] = (uint8_t)a1;
        state_digits[idx][2] = (uint8_t)a2;
        state_digits[idx][3] = (uint8_t)a3;
        state_digits[idx][4] = (uint8_t)a4;
        ++idx;
    }

    /* allocate adjacency structures */
    adjacency = (int **)malloc(STATE_COUNT * sizeof(*adjacency));
    adj_len    = (int *)malloc(STATE_COUNT * sizeof(*adj_len));

    /* pre‑compute transitions */
    for (int s = 0; s < STATE_COUNT; ++s)
    {
        const uint8_t *a = state_digits[s];
        /* use a dynamic array for neighbours of state s */
        int *list = NULL;
        int  len  = 0;
        int  cap  = 0;

        for (int t = 0; t < STATE_COUNT; ++t)
        {
            const uint8_t *b = state_digits[t];
            /* vertical adjacency check: columns must differ */
            bool ok = true;
            for (int j = 0; j < ROWS; ++j)
            {
                if (a[j] == b[j]) { ok = false; break; }
            }
            if (!ok) continue;

            /* 2×2 block constraints */
            for (int j = 0; j < ROWS-1; ++j)
            {
                int diff = (int)a[j] + (int)b[j+1] - ((int)a[j+1] + (int)b[j]);
                if (diff < 0) diff = -diff;
                if (diff != 3) { ok = false; break; }
            }
            if (!ok) continue;

            /* store transition */
            if (len == cap)
            {
                cap = cap ? cap * 2 : 8;
                list = (int *)realloc(list, cap * sizeof(int));
            }
            list[len++] = t;
        }

        adjacency[s] = list;
        adj_len[s]   = len;
    }

    /* allocate DP vectors */
    dp_cur   = (seq_t *)malloc(STATE_COUNT * sizeof(*dp_cur));
    dp_next  = (seq_t *)malloc(STATE_COUNT * sizeof(*dp_next));
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* below offset, undefined */
    init();                                 /* ensure tables are ready */

    /* initialise dp vector: each first row is allowed, count = 1 */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)1;

    /* perform n transitions to add rows 1..n */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        /* propagate counts */
        for (int s = 0; s < STATE_COUNT; ++s)
        {
            const seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;           /* skip zero rows */
            const int *list = adjacency[s];
            const int  len  = adj_len[s];
            for (int k = 0; k < len; ++k)
            {
                int t = list[k];
                dp_next[t] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all endings to get total number of arrays */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) result += dp_cur[i];
    return result;
}

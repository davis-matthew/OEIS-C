/*
 * A223300  4X4X4 triangular graph coloring a rectangular array: number of nX3
 *          0..9 arrays where 0..9 label nodes of a graph with edges 0,1 0,2
 *          1,2 1,3 1,4 2,4 3,4 2,5 4,5 3,6 3,7 4,7 6,7 4,8 5,8 7,8 5,9 8,9
 *          and every array movement to a horizontal or vertical neighbor
 *          moves along an edge of this graph.
 * https://oeis.org/A223300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays of digits 0..9 where each horizontal
 *            and vertical step follows a given 10‑node graph. This is
 *            computed by a transfer‑matrix DP counting rows that are
 *            compatible. The recurrence used by OEIS for n>12 is not needed
 *            for the computation. O(n·S²) time where S=144 is the num...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------------------------------------------------------- */
/*  graph and state data                                       */
/* ---------------------------------------------------------- */
#define MAX_STATES 200          /* 144 admissible rows are enough */

static uint8_t edge[10][10] = {0};   /* adjacency matrix (0/1)          */
static uint16_t state_code[MAX_STATES]; /* state encoded as a*100+b*10+c   */
static size_t S = 0;                 /* actual number of states         */
static uint8_t trans[MAX_STATES][MAX_STATES]; /* transition matrix 0/1 */

/* ---------------------------------------------------------- */
/*  dynamic programming buffers                                */
/* ---------------------------------------------------------- */
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

/* ---------------------------------------------------------- */
/*  memoised answer cache                                       */
/* ---------------------------------------------------------- */
static seq_t *ans = NULL;
static size_t ans_cap = 0;   /* number of allocated elements */
static size_t ans_cnt = 0;   /* maximum n already computed   */

/* ---------------------------------------------------------- */
/*  initialisation routine                                       */
/* ---------------------------------------------------------- */
static void init(void)
{
    /* build adjacency matrix */
    const int edges[][2] = {
        {0,1},{0,2},{1,2},{1,3},{1,4},{2,4},
        {3,4},{2,5},{4,5},{3,6},{3,7},{4,7},
        {6,7},{4,8},{5,8},{7,8},{5,9},{8,9}
    };
    const size_t NE = sizeof(edges)/sizeof(edges[0]);
    for (size_t i = 0; i < NE; ++i) {
        int u = edges[i][0], v = edges[i][1];
        edge[u][v] = edge[v][u] = 1;
    }

    /* enumerate all admissible rows (states) */
    size_t idx = 0;
    for (int a = 0; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b) {
            if (!edge[a][b]) continue;
            for (int c = 0; c <= 9; ++c) {
                if (!edge[b][c]) continue;
                state_code[idx++] = (uint16_t)(a*100 + b*10 + c);
            }
        }
    S = idx;  /* S <= MAX_STATES */

    /* build transition matrix */
    memset(trans, 0, sizeof(trans));
    for (size_t i = 0; i < S; ++i) {
        int a = state_code[i]/100;
        int b = (state_code[i]/10)%10;
        int c = state_code[i]%10;
        for (size_t j = 0; j < S; ++j) {
            int d = state_code[j]/100;
            int e = (state_code[j]/10)%10;
            int f = state_code[j]%10;
            if (edge[a][d] && edge[b][e] && edge[c][f])
                trans[i][j] = 1;
        }
    }

    /* allocate DP vectors */
    dp_curr = (seq_t*)malloc(S * sizeof(seq_t));
    dp_next = (seq_t*)malloc(S * sizeof(seq_t));
    memset(dp_curr, 0, S * sizeof(seq_t));
    memset(dp_next, 0, S * sizeof(seq_t));
    for (size_t i = 0; i < S; ++i)
        dp_curr[i] = 1;   /* every state is allowed as the first column */

    /* initialise answer cache */
    ans_cap = 16;
    ans = (seq_t*)malloc(ans_cap * sizeof(seq_t));
    ans[0] = 0;            /* a(0) is undefined */
    ans[1] = (seq_t)S;     /* a(1) = number of admissible rows = 144 */
    ans_cnt = 1;
}

/* ---------------------------------------------------------- */
/*  public function                                              */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */

    if (!dp_curr) init();           /* lazy initialisation */

    /* if already cached return it */
    if ((size_t)n <= ans_cnt) return ans[(size_t)n];

    /* extend cache up to requested n */
    while (ans_cnt < (size_t)n) {
        /* initialise next column to zero */
        memset(dp_next, 0, S * sizeof(seq_t));

        /* DP transition */
        for (size_t i = 0; i < S; ++i) {
            seq_t val = dp_curr[i];
            if (val == 0) continue;
            const uint8_t *row = trans[i];
            for (size_t j = 0; j < S; ++j) {
                if (row[j]) dp_next[j] += val;
            }
        }

        /* swap dp vectors */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;

        /* compute a(n) = sum of ways for last column */
        seq_t total = 0;
        for (size_t i = 0; i < S; ++i) total += dp_curr[i];

        /* grow answer cache if necessary */
        if (ans_cnt + 1 >= ans_cap) {
            size_t new_cap = ans_cap << 1;
            ans = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
            ans_cap = new_cap;
        }

        ans[++ans_cnt] = total;
    }

    return ans[(size_t)n];
}

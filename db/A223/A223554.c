/*
 * A223554  Petersen graph (3,1) coloring a rectangular array: number of nX6
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal or antidiagonal neighbor moves along an edge of this
 *          graph, with the array starting at 0.
 * https://oeis.org/A223554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq A223554 – Petersen graph (3,1) coloring of an n×6 rectangle
   The implementation uses dynamic programming over all admissible
   horizontal rows.  Each step corresponds to adding a new row that
   satisfies the adjacency constraints with the row above (horizontal
   and antidiagonal).  The number of valid arrays a(n) is obtained
   as the sum of the DP vector for the last row.  Memoisation guarantees
   that repeated calls for any n ≥ 1 are handled in O(n).  The code
   follows the C11 / C++17 standard without using C++-specific
   facilities such as 'new' or templates.  All dynamic allocations
   use the C style cast required to compile under C++.
   -------------------------------------------------------------------- */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------- */
/* The types seq_t and idx_t are supplied by the build environment.    */
/* -------------------------------------------------------------------- */

/* Adjacency matrix for the Petersen graph (3,1).  Entry [i][j] is 1 if
   there is an edge between vertices i and j, 0 otherwise.  Order: 0…5.
   Only horizontal edges are used for row generation.  Antidiagonal
   consistency is checked separately. */
static const int ADJ[6][6] = {
    /* 0 1 2 3 4 5 */
    {0,1,1,1,0,0}, /* 0 */
    {1,0,1,0,1,0}, /* 1 */
    {1,1,0,0,0,1}, /* 2 */
    {1,0,0,0,1,1}, /* 3 */
    {0,1,0,1,0,1}, /* 4 */
    {0,0,1,1,1,0}  /* 5 */
};

/* -------------------------------------------------------------------- */
/* Row generation (finite state machine)                               */
/* -------------------------------------------------------------------- */
static size_t R = 0;              /* number of admissible rows          */
static uint8_t *ROWS = NULL;      /* storage: R × 6 bytes               */
static size_t ROWS_CAP = 0;       /* current capacity for rows          */

static void add_row(const uint8_t r[6])
{
    if (R == ROWS_CAP) {
        size_t new_cap = ROWS_CAP ? ROWS_CAP * 2 : 32;
        uint8_t *tmp = (uint8_t *)realloc(ROWS, new_cap * 6 * sizeof(uint8_t));
        if (!tmp) exit(1);
        ROWS = tmp;
        ROWS_CAP = new_cap;
    }
    memcpy(ROWS + R * 6, r, 6 * sizeof(uint8_t));
    ++R;
}

static void dfs_gen(size_t pos, uint8_t curr[6])
{
    if (pos == 6) {
        add_row(curr);
        return;
    }
    int prev = curr[pos - 1];
    for (int v = 0; v < 6; ++v) {
        if (ADJ[prev][v]) {
            curr[pos] = (uint8_t)v;
            dfs_gen(pos + 1, curr);
        }
    }
}

/* -------------------------------------------------------------------- */
/* Build adjacency lists for antidiagonal compatibility                */
/* -------------------------------------------------------------------- */
static uint16_t *EDGE_FROM = NULL;  /* concatenated adjacency lists   */
static size_t *EDGE_START = NULL;   /* start index of each list       */
static size_t *EDGE_LEN   = NULL;   /* length of each list            */
static size_t EDGE_TOTAL  = 0;      /* total number of edges          */

static inline int diag_ok(size_t s, size_t t)
{
    const uint8_t *rs = ROWS + s * 6;
    const uint8_t *rt = ROWS + t * 6;
    for (int j = 1; j < 6; ++j) {
        if (!ADJ[rs[j - 1]][rt[j]]) return 0;
    }
    return 1;
}

static void build_edges(void)
{
    EDGE_START = (size_t *)malloc(R * sizeof(size_t));
    EDGE_LEN   = (size_t *)malloc(R * sizeof(size_t));
    size_t total = 0;
    for (size_t s = 0; s < R; ++s) {
        size_t cnt = 0;
        for (size_t t = 0; t < R; ++t)
            if (diag_ok(s, t))
                ++cnt;
        EDGE_LEN[s] = cnt;
        total += cnt;
    }
    EDGE_TOTAL = total;
    EDGE_FROM  = (uint16_t *)malloc(total * sizeof(uint16_t));

    size_t acc = 0;
    for (size_t s = 0; s < R; ++s) {
        EDGE_START[s] = acc;
        acc += EDGE_LEN[s];
    }

    for (size_t s = 0; s < R; ++s) {
        size_t idx = EDGE_START[s];
        for (size_t t = 0; t < R; ++t)
            if (diag_ok(s, t))
                EDGE_FROM[idx++] = (uint16_t)t;
    }
}

/* -------------------------------------------------------------------- */
/* DP arrays and cache                                                   */
/* -------------------------------------------------------------------- */
static seq_t *DP_CUR   = NULL;
static seq_t *DP_NEXT  = NULL;
static seq_t *CACHE    = NULL;   /* CACHE[n] == a(n) for 1 ≤ n ≤ CACHE_SZ   */
static size_t CACHE_CAP = 0;
static size_t CACHE_SZ  = 0;      /* maximum n computed so far              */

/* -------------------------------------------------------------------- */
/* Lazy initialisation                                                   */
/* -------------------------------------------------------------------- */
static void init(void)
{
    /* 1. Enumerate all horizontal rows. */
    ROWS_CAP = 32;
    ROWS = (uint8_t *)malloc(ROWS_CAP * 6 * sizeof(uint8_t));
    if (!ROWS) exit(1);

    for (int start = 0; start < 6; ++start) {
        uint8_t cur[6];
        cur[0] = (uint8_t)start;
        dfs_gen(1, cur);
    }

    /* 2. Build a DAG for antidiagonal compatibility. */
    build_edges();

    /* 3. Allocate DP arrays. */
    DP_CUR  = (seq_t *)calloc(R, sizeof(seq_t));
    DP_NEXT = (seq_t *)calloc(R, sizeof(seq_t));

    /* 4. Initialise DP for the first row (must start with 0). */
    for (size_t s = 0; s < R; ++s)
        if (ROWS[s * 6] == 0)
            DP_CUR[s] = (seq_t)1;

    /* 5. Compute a(1). */
    seq_t sum = (seq_t)0;
    for (size_t s = 0; s < R; ++s)
        sum += DP_CUR[s];

    /* 6. Initialise cache. */
    CACHE_CAP = 16;
    CACHE = (seq_t *)calloc(CACHE_CAP, sizeof(seq_t));
    CACHE[1] = sum;
    CACHE_SZ  = 1;
}

/* -------------------------------------------------------------------- */
/* Public sequence function                                             */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!DP_CUR) init();                 /* lazy init on first call */

    if ((size_t)n <= CACHE_SZ) return CACHE[n];

    /* Run DP until the required n is reached. */
    while (CACHE_SZ < (size_t)n) {
        /* Zero next DP buffer. */
        memset(DP_NEXT, 0, R * sizeof(seq_t));

        /* Transition. */
        for (size_t s = 0; s < R; ++s) {
            seq_t val = DP_CUR[s];
            if (!val) continue;
            size_t start = EDGE_START[s];
            size_t len   = EDGE_LEN[s];
            for (size_t k = 0; k < len; ++k) {
                size_t t = EDGE_FROM[start + k];
                DP_NEXT[t] += val;
            }
        }

        /* Sum to get a(current_row). */
        seq_t sum = (seq_t)0;
        for (size_t t = 0; t < R; ++t)
            sum += DP_NEXT[t];

        /* Extend cache if needed. */
        if (CACHE_SZ + 1 >= CACHE_CAP) {
            size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            CACHE = tmp;
            CACHE_CAP = new_cap;
        }
        CACHE[++CACHE_SZ] = sum;

        /* Swap DP buffers. */
        seq_t *tmp = DP_CUR;
        DP_CUR   = DP_NEXT;
        DP_NEXT  = tmp;
    }

    return CACHE[n];
}

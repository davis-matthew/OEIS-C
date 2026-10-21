/*
 * A233899  Number of (n+1)X(3+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 35.
 * https://oeis.org/A233899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A233899
 *  Number of (n+1)×4 0..6 arrays with every 2×2 subblock having the sum of
 *  the squares of the six edge and diagonal differences equal to 35.
 *
 *  This implementation counts the arrays by dynamic programming over all
 *  4‑tuples of values 0..6 (2401 states).  The adjacency graph of states
 *  that can legally follow each other is built once; thereafter each
 *  transition corresponds to adding another row.  The result for each
 *  n is memoised so that repeated calls are O(1).  The method runs in
 *  O(n·E) with E≈10⁴ and easily stays below the 2‑s limit for n≈10 000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ----------  CONSTANTS AND GLOBAL DATA  ----------------------------- */

enum { STATES = 2401 };                /* 7^4 possible rows           */
enum { DEFAULT_MAXN = 10000 };         /* default memoisation size   */

static uint8_t rows[STATES][4];           /* rows[id][col] 0..6          */

struct neigh {
    uint16_t *data;      /* neighbour ids              */
    size_t   size;       /* number of neighbours       */
    size_t   cap;        /* allocated capacity         */
};

static struct neigh adj[STATES];          /* adjacency lists           */

static seq_t dp_prev[STATES];             /* DP for previous height    */
static seq_t dp_next[STATES];             /* DP for new height         */

static seq_t *memo = NULL;                /* memoised a(n) values       */
static size_t memo_cap = 0;               /* current memo capacity     */
static size_t memo_size = 0;              /* number of terms stored    */

/* ----------  HELPER TO ADD A NEIGHBOUR -------------------------------- */

static void add_neigh(int id, uint16_t nb)
{
    struct neigh *p = &adj[id];
    if (p->size == p->cap) {
        size_t newcap = p->cap ? p->cap << 1 : 8;
        uint16_t *tmp = (uint16_t *)realloc(p->data, newcap * sizeof(uint16_t));
        if (!tmp) abort();               /* unrecoverable allocation error */
        p->data = tmp;
        p->cap  = newcap;
    }
    p->data[p->size++] = nb;
}

/* ----------  INITIALISATION OF ALL STATIC DATA ------------------------ */

static void init_data(void)
{
    static int inited = 0;
    if (inited) return;
    inited = 1;

    /* generate all 4‑tuples of digits 0..6 in base‑7 order */
    for (int id = 0; id < STATES; ++id) {
        int x = id;
        for (int c = 3; c >= 0; --c) {
            rows[id][c] = (uint8_t)(x % 7);
            x /= 7;
        }
    }

    /* build adjacency lists */
    for (int i = 0; i < STATES; ++i) {
        adj[i].data = NULL;
        adj[i].size = 0;
        adj[i].cap  = 0;
        for (int j = 0; j < STATES; ++j) {
            int ok = 1;
            for (int k = 0; k < 3 && ok; ++k) {
                int a = rows[i][k];
                int b = rows[i][k+1];
                int c = rows[j][k];
                int d = rows[j][k+1];
                int sum = (a-b)*(a-b) + (c-d)*(c-d)
                        + (a-c)*(a-c) + (b-d)*(b-d)
                        + (a-d)*(a-d) + (b-c)*(b-c);
                if (sum != 35) ok = 0;
            }
            if (ok) add_neigh(i, (uint16_t)j);
        }
    }

    /* initialise DP for height 1 (any row is allowed) */
    for (int i = 0; i < STATES; ++i) dp_prev[i] = 1;

    /* allocate memo table */
    memo_cap  = DEFAULT_MAXN + 1;
    memo      = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    if (!memo) abort();
    memo_size = 0;
}

/* ----------  PUBLIC FUNCTION  ---------------------------------------- */

seq_t a(idx_t n)
{
    init_data();                       /* guarantees data ready */

    /*  the sequence is defined for n ≥ 1; values for n < 1 are ignored   */
    if (n == 0) return (seq_t)0;

    /* extend memo array if necessary */
    if (n > memo_cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        memo_cap = newcap;
        /* new slots are already zero-initialised by realloc behaviour   */
    }

    /* if requested value already computed, return it immediately */
    if (n <= memo_size) return memo[n-1];

    /* perform DP steps up to the desired n */
    idx_t steps = memo_size;            /* steps already processed */

    while (steps < n) {
        /* clear dp_next */
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        /* transition */
        for (int s = 0; s < STATES; ++s) {
            if (dp_prev[s] == 0) continue;
            seq_t val = dp_prev[s];
            for (size_t t = 0; t < adj[s].size; ++t) {
                uint16_t nb = adj[s].data[t];
                dp_next[nb] += val;
            }
        }

        /* compute a(steps+1) as total number of arrays of height steps+2 */
        seq_t sum = 0;
        for (int i = 0; i < STATES; ++i) sum += dp_next[i];
        memo[steps] = sum;            /* store result for n = steps+1 */

        /* prepare for next iteration */
        for (int i = 0; i < STATES; ++i) dp_prev[i] = dp_next[i];
        ++steps;
    }

    memo_size = n;
    return memo[n-1];
}

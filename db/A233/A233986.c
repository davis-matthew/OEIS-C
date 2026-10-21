/*
 * A233986  Number of (n+1)X(5+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = number of (2×6) arrays, for n≥1
   a(n+1) = Σ_i Σ_{j∈Adj(i)} dp_n[i],
   where dp_0[i] = 1 for each admissible row i.
   This DP enumerates all (n+1)×6 arrays satisfying the local
   edge–difference condition and no equal adjacent entries. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROWS 5120                 /* 5^6 with no equal adjacent digits */

/* Global data – built only once */
static bool          initialized = false;
static size_t        S = 0;          /* number of admissible rows */
static uint8_t       rows[MAX_ROWS][6];
static int          *adj[MAX_ROWS];
static size_t        adj_len[MAX_ROWS];
static size_t        adj_cap[MAX_ROWS];

/* Append an edge j to adjacency list of state i */
static void add_edge(size_t i, int j)
{
    if (adj_len[i] == adj_cap[i]) {
        size_t new_cap = adj_cap[i] ? adj_cap[i] * 2 : 4;
        int *tmp = (int *)realloc(adj[i], new_cap * sizeof(int));
        if (!tmp) abort();           /* memory failure */
        adj[i]  = tmp;
        adj_cap[i] = new_cap;
    }
    adj[i][adj_len[i]++] = j;
}

/* Build admissible rows and adjacency relation once */
static void init(void)
{
    size_t idx = 0;

    /* enumerate all 6‑digit base‑5 numbers with no equal adjacent digits */
    for (int code = 0; code < 15625; ++code) {      /* 5^6 */
        int tmp = code;
        uint8_t d[6];
        for (int k = 0; k < 6; ++k) {
            d[k] = (uint8_t)(tmp % 5);
            tmp /= 5;
        }

        bool ok = true;
        for (int k = 0; k < 5; ++k) {
            if (d[k] == d[k + 1]) { ok = false; break; }
        }
        if (!ok) continue;

        memcpy(rows[idx], d, 6 * sizeof(uint8_t));
        ++idx;
    }
    S = idx;

    for (size_t i = 0; i < S; ++i) {
        adj[i] = NULL;
        adj_len[i] = 0;
        adj_cap[i] = 0;
    }

    /* build adjacency table: pair of rows satisfies the 2×2 condition */
    for (size_t i = 0; i < S; ++i) {
        for (size_t j = 0; j < S; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                if (rows[i][k] == rows[j][k]) { ok = false; break; }
                int a = (int)rows[i][k];
                int b = (int)rows[i][k + 1];
                int c = (int)rows[j][k];
                int d = (int)rows[j][k + 1];

                int da = a > b ? a - b : b - a;
                int db = c > d ? c - d : d - c;
                int dc = a > c ? a - c : c - a;
                int dd = b > d ? b - d : d - b;
                int sum = da * da + db * db + dc * dc + dd * dd;
                if (sum != 10) { ok = false; break; }
            }
            if (ok) add_edge(i, (int)j);
        }
    }

    initialized = true;
}

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* allocate DP arrays */
    seq_t *curr = (seq_t *)calloc(S, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(S, sizeof(seq_t));
    if (!curr || !next) abort();  /* memory failure */

    /* first row: any admissible row is possible */
    for (size_t i = 0; i < S; ++i) curr[i] = 1;

    /* perform n transitions → obtain arrays of height n+1 */
    for (idx_t step = 0; step < n; ++step) {
        memset(next, 0, S * sizeof(seq_t));
        for (size_t i = 0; i < S; ++i) {
            seq_t vi = curr[i];
            if (!vi) continue;
            int *list = adj[i];
            size_t deg = adj_len[i];
            for (size_t k = 0; k < deg; ++k) {
                int j = list[k];
                next[j] += vi;
            }
        }
        /* swap arrays */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* total number of arrays is sum over final row choices */
    seq_t total = 0;
    for (size_t i = 0; i < S; ++i) total += curr[i];

    free(curr);
    free(next);
    return total;
}

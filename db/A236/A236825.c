/*
 * A236825  Number of (n+1)X(5+1) 0..2 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A236825: number of (n+1)×6 arrays over {0,1,2} with the
 * property that for every 2×2 subblock the value
 *   max + min – lower median
 * equals a constant.
 *
 * This implementation builds the adjacency graph of admissible row
 * pairs for each constant C∈{0,1,2} and then counts, via dynamic
 * programming, all sequences of n+1 rows.  The result is summed
 * over all constants.  No closed form is known, so this is the
 * direct algorithm.  It is O(n·E) where E≈729²/2≈2.6×10⁵, easily
 * fast for n≤10000.  The code also works when seq_t is an
 * arbitrary‑precision integer class, because only +, ==, and
 * assignment are used on seq_t values.  For the C++ build the
 * required casts from void* to int* are added. */

#include <stdlib.h>
#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    int *list;
} RowAdj;

/* pre‑computed data */
static uint8_t fvals[81];                 /* fvalue for each 4‑tuple */
static unsigned char row_digit[729][6];   /* digits of a row in base‑3 */
static RowAdj adjacency[3][729];          /* adjacency list for each C */
static int adjacency_built = 0;

/* add an integer to a dynamic list */
static void add_edge(RowAdj *adj, int v)
{
    if (adj->len == adj->cap) {
        size_t newcap = adj->cap ? adj->cap << 1 : 4;
        adj->list = (int *)realloc(adj->list, newcap * sizeof(int));
        adj->cap = newcap;
    }
    adj->list[adj->len++] = v;
}

/* pre–compute f(a,b,c,d) for all 4‑tuples of {0,1,2} */
static void init_fvals(void)
{
    for (int a = 0; a < 3; ++a)
    for (int b = 0; b < 3; ++b)
    for (int c = 0; c < 3; ++c)
    for (int d = 0; d < 3; ++d) {
        int s0 = a, s1 = b, s2 = c, s3 = d;
        /* simple 4‑element sort */
        if (s0 > s1) { int t = s0; s0 = s1; s1 = t; }
        if (s2 > s3) { int t = s2; s2 = s3; s3 = t; }
        if (s0 > s2) { int t = s0; s0 = s2; s2 = t; }
        if (s1 > s3) { int t = s1; s1 = s3; s3 = t; }
        if (s1 > s2) { int t = s1; s1 = s2; s2 = t; }
        int f = s3 + s0 - s1;
        int idx = a + 3*b + 9*c + 27*d;
        fvals[idx] = (uint8_t)f;
    }
}

/* pre‑compute the base‑3 digits of all 6‑tuples (rows) */
static void init_row_digits(void)
{
    for (int r = 0; r < 729; ++r) {
        int x = r;
        for (int pos = 0; pos < 6; ++pos) {
            row_digit[r][pos] = (unsigned char)(x % 3);
            x /= 3;
        }
    }
}

/* build adjacency lists for each constant C */
static void build_adjacency(void)
{
    init_fvals();
    init_row_digits();

    for (int c = 0; c < 3; ++c) {
        /* initialise lists */
        for (int r = 0; r < 729; ++r) {
            adjacency[c][r].len = 0;
            adjacency[c][r].cap = 4;
            adjacency[c][r].list = (int *)malloc(4 * sizeof(int));
        }

        /* self‑loops */
        for (int u = 0; u < 729; ++u) {
            int ok = 1;
            for (int j = 0; j < 5; ++j) {
                int a = row_digit[u][j];
                int b = row_digit[u][j+1];
                int c1 = row_digit[u][j];
                int d = row_digit[u][j+1];
                int idx = a + 3*b + 9*c1 + 27*d;
                if (fvals[idx] != (uint8_t)c) { ok = 0; break; }
            }
            if (ok) add_edge(&adjacency[c][u], u);
        }

        /* pairs u < v */
        for (int u = 0; u < 729; ++u) {
            for (int v = u + 1; v < 729; ++v) {
                int ok = 1;
                for (int j = 0; j < 5; ++j) {
                    int a = row_digit[u][j];
                    int b = row_digit[u][j+1];
                    int c1 = row_digit[v][j];
                    int d = row_digit[v][j+1];
                    int idx = a + 3*b + 9*c1 + 27*d;
                    if (fvals[idx] != (uint8_t)c) { ok = 0; break; }
                }
                if (ok) {
                    add_edge(&adjacency[c][u], v);
                    add_edge(&adjacency[c][v], u);
                }
            }
        }
    }
    adjacency_built = 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    if (!adjacency_built) build_adjacency();

    static seq_t dp1[729], dp2[729];
    seq_t total = 0;

    for (int C = 0; C < 3; ++C) {
        seq_t *cur = dp1;
        seq_t *next = dp2;
        /* start with a single row */
        for (int i = 0; i < 729; ++i) cur[i] = 1;

        /* perform n transitions to obtain n+1 rows */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 729; ++i) next[i] = 0;
            for (int v = 0; v < 729; ++v) {
                RowAdj *adjv = &adjacency[C][v];
                seq_t sum = 0;
                for (size_t k = 0; k < adjv->len; ++k) {
                    int u = adjv->list[k];
                    sum += cur[u];
                }
                next[v] = sum;
            }
            seq_t *tmp = cur; cur = next; next = tmp;
        }

        /* accumulate total for this constant */
        seq_t sumC = 0;
        for (int i = 0; i < 729; ++i) sumC += cur[i];
        total += sumC;
    }
    return total;
}

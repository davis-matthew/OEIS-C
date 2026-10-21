/*
 * A205923  Number of (n+1)X6 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205923: Number of (n+1)×6 arrays over {0,1,2,3}
   whose every 2×2 sub‑block has exactly one clockwise edge increase
   and non‑zero determinant.  The values are computed by a transfer‑matrix
   dynamic programming over the 6‑digit rows. */

/*  The build supplies the types seq_t and idx_t.  seq_t may be a
    built‑in unsigned integer or an arbitrary‑precision integer.
    Only integer arithmetic and simple loops are used. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Helper structures for adjacency lists ---------- */
typedef struct {
    uint16_t *data;   /* array of destination indices */
    size_t    len;    /* number of stored indices   */
    size_t    cap;    /* current allocated capacity */
} vec_t;

static vec_t adj[4096];          /* adjacency list for each source row */
static int   adj_built = 0;      /* flag to build the graph once */

/* Count of 4‑cell block validity (pre‑computed lookup). */
static uint8_t block_ok[16][16]; /* indexed by (a,b) and (c,d) */

/* ---------- Initialization of the block validity table ---------- */
static void init_block_table(void)
{
    for (int a = 0; a < 4; ++a)
    for (int b = 0; b < 4; ++b)
    for (int c = 0; c < 4; ++c)
    for (int d = 0; d < 4; ++d) {
        int inc = (b > a) + (d > b) + (c > d) + (a > c);
        int det = a * d - b * c;
        block_ok[a*4+b][c*4+d] = (inc == 1 && det != 0) ? 1 : 0;
    }
}

/* ---------- Build all adjacency lists ---------- */
static void build_adjacency(void)
{
    int i, j, k, l;
    uint8_t digits[4096][6];

    /* Pre‑compute all 6‑digit rows in base‑4 */
    for (int r = 0; r < 4096; ++r) {
        int tmp = r;
        for (int c = 0; c < 6; ++c) {
            digits[r][c] = tmp & 3;
            tmp >>= 2;
        }
    }

    /* Initialise adjacency vectors */
    for (i = 0; i < 4096; ++i) {
        adj[i].cap = 4;
        adj[i].len = 0;
        adj[i].data = (uint16_t *)malloc(4 * sizeof(uint16_t));
    }

    /* For every pair of rows, test whether they are compatible */
    for (int r = 0; r < 4096; ++r) {
        for (int s = 0; s < 4096; ++s) {
            int ok = 1;
            for (k = 0; k < 5 && ok; ++k) {
                int a = digits[r][k];
                int b = digits[r][k + 1];
                int c = digits[s][k];
                int d = digits[s][k + 1];
                if (!block_ok[a*4+b][c*4+d]) ok = 0;
            }
            if (ok) {
                vec_t *v = &adj[r];
                if (v->len == v->cap) {
                    v->cap <<= 1;
                    v->data = (uint16_t *)realloc(v->data, v->cap * sizeof(uint16_t));
                }
                v->data[v->len++] = (uint16_t)s;
            }
        }
    }
    adj_built = 1;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cnt[2];
    static int counts_built = 0;

    /* Build the transfer matrix once */
    if (!adj_built) {
        init_block_table();
        build_adjacency();
    }

    /* Allocate two count arrays once */
    if (!counts_built) {
        cnt[0] = (seq_t *)calloc(4096, sizeof(seq_t));
        cnt[1] = (seq_t *)calloc(4096, sizeof(seq_t));
        if (!cnt[0] || !cnt[1]) return (seq_t)0; /* allocation failure */
        counts_built = 1;
    }

    seq_t *cur = cnt[0];
    seq_t *nxt = cnt[1];

    /* Initial counts: one way to start with any row */
    for (size_t i = 0; i < 4096; ++i) cur[i] = 1;

    /* Perform n transitions (n steps) */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next array */
        for (size_t i = 0; i < 4096; ++i) nxt[i] = 0;

        for (size_t src = 0; src < 4096; ++src) {
            seq_t val = cur[src];
            if (val == 0) continue;
            const vec_t *v = &adj[src];
            for (size_t k = 0; k < v->len; ++k) {
                uint16_t dst = v->data[k];
                nxt[dst] += val;
            }
        }
        /* Swap current and next */
        seq_t *tmp = cur;
        cur = nxt; nxt = tmp;
    }

    /* Sum all counts: total number of arrays of size (n+1)×6 */
    seq_t total = 0;
    for (size_t i = 0; i < 4096; ++i) total += cur[i];
    return total;
}

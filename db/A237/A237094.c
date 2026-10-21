/*
 * A237094  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock equal.
 * https://oeis.org/A237094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = \sum_{v=0}^{4} \#\{(r_0,\dots ,r_{n})\mid
 * r_i\in[0..80],\;
 * \text{block value}(r_{k-1},r_k)=v\ \forall k=1..n\},
 *  where r_i represents a row of length 4 over {0,1,2}
 *  and \text{block value} = max+lower‑median−upper‑median of each 2×2 block.
 */
#include <stdbool.h>
#include <string.h>

#define ROWS    81          /* 3^4 possible rows */
#define V_MAX   5           /* possible block values 0..4 */
#define MAX_NEXT ROWS       /* maximum outgoing per row and per value */

/* digits of each row (base‑3 representation) */
static unsigned char row_digits[ROWS][4];

/* adjacency lists: next_rows[v][p][k] = q (next row) */
static unsigned char next_rows[V_MAX][ROWS][MAX_NEXT];
static unsigned char next_cnt[V_MAX][ROWS];

static bool tables_built = false;

/* sorting network for a 4‑tuple to find max, lower median and upper median */
static int block_value(int a, int b, int c, int d)
{
    int v[4] = {a, b, c, d};
    if (v[0] > v[1]) { int t = v[0]; v[0] = v[1]; v[1] = t; }
    if (v[2] > v[3]) { int t = v[2]; v[2] = v[3]; v[3] = t; }
    if (v[0] > v[2]) { int t = v[0]; v[0] = v[2]; v[2] = t; }
    if (v[1] > v[3]) { int t = v[1]; v[1] = v[3]; v[3] = t; }
    if (v[1] > v[2]) { int t = v[1]; v[1] = v[2]; v[2] = t; }

    return v[3] + v[1] - v[2];
}

/* build adjacency tables once */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* precompute row digits */
    for (int code = 0; code < ROWS; ++code) {
        int tmp = code;
        for (int pos = 0; pos < 4; ++pos) {
            row_digits[code][pos] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* build transitions for each possible block value v */
    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            int v0 = block_value(row_digits[p][0], row_digits[p][1],
                                  row_digits[q][0], row_digits[q][1]);
            int v1 = block_value(row_digits[p][1], row_digits[p][2],
                                  row_digits[q][1], row_digits[q][2]);
            int v2 = block_value(row_digits[p][2], row_digits[p][3],
                                  row_digits[q][2], row_digits[q][3]);
            if (v0 == v1 && v1 == v2) {
                int v = v0;
                unsigned char k = next_cnt[v][p];
                next_rows[v][p][k++] = (unsigned char)q;
                next_cnt[v][p] = k;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* sequence undefined for n<1 */

    build_tables();
    const idx_t h = n + 1;            /* number of rows in the array */

    static seq_t dp[ROWS];
    static seq_t nxt[ROWS];

    seq_t result = (seq_t)0;

    for (int v = 0; v < V_MAX; ++v) {
        /* initialise: any first row is allowed */
        for (int i = 0; i < ROWS; ++i) dp[i] = (seq_t)1;

        seq_t *cur = dp;
        seq_t *next = nxt;

        for (idx_t step = 1; step < h; ++step) {
            /* zero next */
            for (int i = 0; i < ROWS; ++i) next[i] = (seq_t)0;

            for (int p = 0; p < ROWS; ++p) {
                seq_t cur_val = cur[p];
                if (!cur_val) continue;
                unsigned char k = next_cnt[v][p];
                for (unsigned char t = 0; t < k; ++t) {
                    int q = next_rows[v][p][t];
                    next[q] = next[q] + cur_val;
                }
            }
            /* swap */
            seq_t *tmp = cur;
            cur = next;
            next = tmp;
        }

        /* sum counts for this value v */
        seq_t cnt = (seq_t)0;
        for (int i = 0; i < ROWS; ++i) cnt = cnt + cur[i];
        result = result + cnt;
    }

    return result;
}

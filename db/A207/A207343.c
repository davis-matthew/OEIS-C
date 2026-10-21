/*
 * A207343  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×5 binary arrays avoiding horizontal
 * 000/011 and vertical 000/111.
 * a(1)=13, a(2)=169, a(3)=426, …
 * 
 * The algorithm keeps only the last two rows.
 * The state is a pair of legal 5‑bit rows (x,y).
 * For each state (x,y) we precompute all legal rows z that can follow
 * without producing a vertical triple.
 * The number of arrays of length k is updated by a simple DP,
 * O(n * V²) with V=13.  For achievable n this is trivial.
 */

#include <stdint.h>

/* Global tables for legal rows and transitions */
static int _valid_rows[32];
static int _valid_cnt;
static int _trans[13][13][13];
static int _trans_len[13][13];
static int _initialized = 0;

/* Precompute all legal rows and transitions */
static void _init(void)
{
    int i, j, k;
    /* Find all rows that avoid horizontal 000 or 011 */
    for (i = 0; i < 32; ++i) {
        int ok = 1;
        for (j = 0; j <= 2; ++j) {
            int a = (i >> j)     & 1;
            int b = (i >> (j+1)) & 1;
            int c = (i >> (j+2)) & 1;
            if ((a==0 && b==0 && c==0) || (a==0 && b==1 && c==1)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            _valid_rows[_valid_cnt++] = i;
        }
    }

    /* Build transitions: for each (i,j) find all k that avoid vertical 000/111 */
    for (i = 0; i < _valid_cnt; ++i) {
        for (j = 0; j < _valid_cnt; ++j) {
            int cnt = 0;
            for (k = 0; k < _valid_cnt; ++k) {
                int good = 1;
                for (int pos = 0; pos < 5; ++pos) {
                    int a = (_valid_rows[i] >> pos) & 1;
                    int b = (_valid_rows[j] >> pos) & 1;
                    int c = (_valid_rows[k] >> pos) & 1;
                    if (a == b && b == c) { good = 0; break; }
                }
                if (good) {
                    _trans[i][j][cnt++] = k;
                }
            }
            _trans_len[i][j] = cnt;
        }
    }
    _initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!_initialized) _init();

    if (n == 0) return 0;                        /* below offset – undefined */
    if (n == 1) return (seq_t)_valid_cnt;        /* 13 */
    if (n == 2) return (seq_t)_valid_cnt * (seq_t)_valid_cnt; /* 169 */

    static seq_t dp_prev[169];
    static seq_t dp_next[169];
    int V = _valid_cnt;
    int total_states = V * V;
    int idx, i, j, k, t, len;

    /* Initialise dp_prev for length 2: all pairs allowed (value 1) */
    for (idx = 0; idx < total_states; ++idx) {
        dp_prev[idx] = (seq_t)0;
    }
    for (i = 0; i < V; ++i) {
        for (j = 0; j < V; ++j) {
            dp_prev[i * V + j] = (seq_t)1;
        }
    }

    /* Iterate rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_next */
        for (idx = 0; idx < total_states; ++idx) {
            dp_next[idx] = (seq_t)0;
        }

        /* DP transition */
        for (i = 0; i < V; ++i) {
            for (j = 0; j < V; ++j) {
                seq_t cur = dp_prev[i * V + j];
                if (cur == (seq_t)0) continue;
                len = _trans_len[i][j];
                for (t = 0; t < len; ++t) {
                    k = _trans[i][j][t];
                    int pos = j * V + k;
                    dp_next[pos] += cur;
                }
            }
        }
        /* swap dp_prev and dp_next */
        for (idx = 0; idx < total_states; ++idx) {
            dp_prev[idx] = dp_next[idx];
        }
    }

    /* Sum over all ending pairs */
    seq_t ans = (seq_t)0;
    for (idx = 0; idx < total_states; ++idx) {
        ans += dp_prev[idx];
    }
    return ans;
}

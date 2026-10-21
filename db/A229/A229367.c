/*
 * A229367  Number of nX3 0..2 arrays avoiding 11 horizontally, 22 vertically
 *          and 00 antidiagonally.
 * https://oeis.org/A229367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} avoiding 11
 *            horizontally, 22 vertically and 00 anti‑diagonally. Computed via
 *            transfer‑matrix dynamic programming
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool valid_state[27];          /* state is 0..26 (base‑3) */
static int  state_map[27];            /* map state → index 0..k‑1 (-1 if invalid) */
static int  idx_to_state[27];         /* index → state value */
static bool adjacency[27][27];        /* transition matrix between indices */
static int  k;                        /* number of valid states */

static seq_t dp[27];
static seq_t ndp[27];
static idx_t curr_len = 0;            /* length for which dp holds (last computed) */

static seq_t *res_arr = NULL;         /* cached sequence values, res_arr[i] = a(i) */
static size_t res_cap = 0;            /* allocated capacity of res_arr */

/* ------------------------------------------------------------------ */
/* Build the state graph and initialise length‑1 DP */
static void init_graph(void)
{
    int count = 0;
    for (int st = 0; st < 27; ++st) {
        int d[3];
        int tmp = st;
        bool bad = false;
        for (int r = 0; r < 3; ++r) {
            d[r] = tmp % 3;
            tmp /= 3;
        }
        /* vertical restriction: no 22 */
        if (d[0] == 2 && d[1] == 2) bad = true;
        if (d[1] == 2 && d[2] == 2) bad = true;
        valid_state[st] = !bad;
        if (bad) {
            state_map[st] = -1;
        } else {
            state_map[st] = count;
            idx_to_state[count] = st;
            ++count;
        }
    }
    k = count;

    /* adjacency matrix */
    for (int i = 0; i < k; ++i) {
        int s = idx_to_state[i];
        int si[3], sj[3];
        int tmp = s;
        for (int r = 0; r < 3; ++r) { si[r] = tmp % 3; tmp /= 3; }
        for (int j = 0; j < k; ++j) {
            int t = idx_to_state[j];
            int tmp2 = t;
            for (int r = 0; r < 3; ++r) { sj[r] = tmp2 % 3; tmp2 /= 3; }

            bool ok = true;
            /* horizontal 11 */
            for (int r = 0; r < 3; ++r) {
                if (si[r] == 1 && sj[r] == 1) { ok = false; break; }
            }
            if (!ok) { adjacency[i][j] = false; continue; }

            /* anti‑diagonal 00: (row r+1 in left) with (row r in right) */
            for (int r = 0; r < 2; ++r) {
                if (si[r+1] == 0 && sj[r] == 0) { ok = false; break; }
            }
            adjacency[i][j] = ok;
        }
    }

    /* DP for length 1 */
    for (int i = 0; i < k; ++i) dp[i] = (seq_t)1;
    curr_len = 1;

    /* allocate initial results array */
    res_cap = 4; /* small initial capacity */
    res_arr = (seq_t*)malloc(res_cap * sizeof(seq_t));
    if (!res_arr) return;
    res_arr[0] = (seq_t)0;

    seq_t sum = (seq_t)0;
    for (int i = 0; i < k; ++i) sum += dp[i];
    res_arr[1] = sum;
}

/* ------------------------------------------------------------------ */
/* Extend DP up to length n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (!res_arr) init_graph();
    if (!res_arr) return;  /* failed to initialise */

    if (n <= (idx_t)curr_len) return;

    /* grow results array if needed */
    size_t needed = (size_t)(n + 1);
    if (needed > res_cap) {
        size_t new_cap = res_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(res_arr, new_cap * sizeof(seq_t));
        if (!tmp) return;
        res_arr = tmp;
        res_cap = new_cap;
    }

    for (idx_t len = (idx_t)(curr_len + 1); len <= n; ++len) {
        /* compute ndp from dp */
        for (int i = 0; i < k; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < k; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            for (int t = 0; t < k; ++t) {
                if (!adjacency[s][t]) continue;
                ndp[t] = ndp[t] + val;
            }
        }
        for (int i = 0; i < k; ++i) dp[i] = ndp[i];

        /* sum of dp gives a(len) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < k; ++i) sum += dp[i];
        res_arr[len] = sum;
    }
    curr_len = (int)n;
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_up_to(n);
    return res_arr[n];
}

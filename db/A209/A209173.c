/*
 * A209173  Number of nX1 0..3 arrays with every element value z a city block
 *          distance of exactly z from another element value z.
 * https://oeis.org/A209173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The DP described below counts the number of n‑by‑1 arrays *
 *            whose entries are in {0,1,2,3} and each element of value z is at
 *            * distance exactly z from another element of the same value. *
 *            The algorithm keeps, for the last three positions, the value
 *            (0…3 * or 4 for an empty slot) and a flag indica...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_VAL   5          /* 0..3 plus sentinel 4 for empty */
#define MAX_NEED  2          /* 0 or 1 */
#define MAX_STATES ((MAX_VAL * MAX_NEED) * (MAX_VAL * MAX_NEED) * (MAX_VAL * MAX_NEED)) /* 1000 */

static void init_state_tables(void);
static inline int encode_state(int v0, int n0, int v1, int n1, int v2, int n2);

static int    v0_arr[MAX_STATES];
static int    v1_arr[MAX_STATES];
static int    v2_arr[MAX_STATES];
static int    n0_arr[MAX_STATES];
static int    n1_arr[MAX_STATES];
static int    n2_arr[MAX_STATES];
static bool   allzero_arr[MAX_STATES];   /* true if all need flags are zero for this state */

static seq_t *memo = NULL;
static size_t memo_cap = 0;   /* number of elements allocated in memo (indexable 0..n) */
static idx_t  cur_len = 0;    /* maximum length already processed */

static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];

static bool tables_inited = false;

/* ------------------------------------------------------------------------- */

static inline int encode_state(int v0, int n0, int v1, int n1, int v2, int n2)
{
    return ((((v0 * MAX_NEED + n0) * MAX_VAL + v1) * MAX_NEED + n1) * MAX_VAL + v2) * MAX_NEED + n2;
}

static void init_state_tables(void)
{
    if (tables_inited) return;
    int idx = 0;
    for (int a = 0; a < MAX_VAL; ++a)
        for (int b = 0; b < MAX_NEED; ++b)
            for (int c = 0; c < MAX_VAL; ++c)
                for (int d = 0; d < MAX_NEED; ++d)
                    for (int e = 0; e < MAX_VAL; ++e)
                        for (int f = 0; f < MAX_NEED; ++f) {
                            v0_arr[idx] = a; v1_arr[idx] = c; v2_arr[idx] = e;
                            n0_arr[idx] = b; n1_arr[idx] = d; n2_arr[idx] = f;
                            allzero_arr[idx] = (b == 0 && d == 0 && f == 0);
                            ++idx;
                        }
    tables_inited = true;
}

/* ------------------------------------------------------------------------- */

static void ensure_memo(size_t n)
{
    if (n < memo_cap) return;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = memo_cap; i < new_cap; ++i) tmp[i] = 0;
    memo = tmp;
    memo_cap = new_cap;
}

/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_state_tables();

    /* Allocate memo the first time */
    if (!memo) {
        memo_cap = 1;
        memo = (seq_t *)calloc(memo_cap, sizeof(seq_t));
        if (!memo) abort();
    }

    /* Initialise DP for length 0 (empty array) once */
    if (cur_len == 0) {
        for (int i = 0; i < MAX_STATES; ++i) dp_cur[i] = 0;
        int init_state = encode_state(4, 0, 4, 0, 4, 0);   /* three empty slots */
        dp_cur[init_state] = 1;
        memo[0] = 1;     /* value for length 0 (unused) */
    }

    /* Extend memoisation up to requested n */
    while (cur_len < n) {
        ++cur_len;

        /* zero next table */
        for (int i = 0; i < MAX_STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < MAX_STATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (!cnt) continue;

            int v0 = v0_arr[s], v1 = v1_arr[s], v2 = v2_arr[s];
            int need0 = n0_arr[s], need1 = n1_arr[s], need2 = n2_arr[s];

            for (int nv = 0; nv <= 3; ++nv) {
                int n0 = need0, n1 = need1, n2 = need2;

                /* Update need flags for older positions */
                if (v0 == nv && v0 == 3) n0 = 0;
                if (v1 == nv && v1 == 2) n1 = 0;
                if (v2 == nv && v2 == 1) n2 = 0;

                /* If oldest position still needs partner, this transition is invalid */
                if (n0 != 0) continue;

                /* Determine need flag for new position */
                int new_need = (nv == 0) ? 0 : 1;
                if (nv != 0) {
                    if ((v0 == nv && nv == 3) ||
                        (v1 == nv && nv == 2) ||
                        (v2 == nv && nv == 1))
                        new_need = 0;
                }

                /* Shift window: drop position i-3, move others, add new position */
                int nv0 = v1;       int nn0 = n1;
                int nv1 = v2;       int nn1 = n2;
                int nv2 = nv;       int nn2 = new_need;

                int ns = encode_state(nv0, nn0, nv1, nn1, nv2, nn2);
                dp_next[ns] += cnt;
            }
        }

        /* Move dp_next to dp_cur for next iteration */
        for (int i = 0; i < MAX_STATES; ++i) dp_cur[i] = dp_next[i];

        /* Compute total for current length */
        seq_t total = 0;
        for (int i = 0; i < MAX_STATES; ++i)
            if (allzero_arr[i])
                total += dp_cur[i];

        ensure_memo(cur_len);
        memo[cur_len] = total;
    }

    return memo[n];
}

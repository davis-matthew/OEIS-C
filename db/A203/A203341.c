/*
 * A203341  Number of nX3 0..4 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays of digits 0..4 such that every
 *            non‑zero element is <= at least two of its horizontal and
 *            vertical neighbours (OEIS A203341)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Number of possible 3‑digit patterns for a row (5^3). */
#define NUM_PAT 125
/* Requirement mask uses 3 bits (one per column). */
#define NUM_MASK 8

/* Pre‑computed tables. */
static int vals[NUM_PAT][3];          /* digits of each pattern   */
static int mask_top[NUM_PAT];         /* requirement mask for top row; -1 if impossible */
static int req_pair[NUM_PAT][NUM_PAT];/* requirement mask for a row given its upper neighbour; -1 if impossible */
static int next_allowed_cnt[NUM_PAT][NUM_MASK];
static int *next_allowed[NUM_PAT][NUM_MASK];

/* Compute mask for a row that has no upper neighbour. */
static int compute_req_mask_top(int cur_ofs)
{
    int m = 0;
    for (int j = 0; j < 3; ++j) {
        int v = vals[cur_ofs][j];
        if (v == 0) continue;          /* zeros impose no requirement */
        int ge = 0;
        if (j > 0 && vals[cur_ofs][j-1] >= v) ++ge;
        if (j < 2 && vals[cur_ofs][j+1] >= v) ++ge;
        if (ge >= 2) {
            /* satisfied by horizontal neighbours alone */
        } else if (ge == 1) {
            m |= 1 << j;                /* need down neighbour to be >= v */
        } else {                         /* ge == 0, impossible */
            return -1;
        }
    }
    return m;
}

/* Compute mask for a row that has an upper neighbour at pattern up_ofs. */
static int compute_req_mask_with_up(int up_ofs, int cur_ofs)
{
    int m = 0;
    for (int j = 0; j < 3; ++j) {
        int v = vals[cur_ofs][j];
        if (v == 0) continue;          /* zeros impose no requirement */
        int ge = 0;
        if (j > 0 && vals[cur_ofs][j-1] >= v) ++ge;
        if (j < 2 && vals[cur_ofs][j+1] >= v) ++ge;
        if (vals[up_ofs][j] >= v) ++ge; /* vertical neighbour above */
        if (ge >= 2) {
            /* satisfied already */
        } else if (ge == 1) {
            m |= 1 << j;                /* need down neighbour to be >= v */
        } else {                         /* ge == 0, impossible */
            return -1;
        }
    }
    return m;
}

/* Pre‑compute all tables used by the DP. */
static void precompute(void)
{
    /* Enumerate all 3‑digit patterns (base 5). */
    for (int p = 0; p < NUM_PAT; ++p) {
        int x = p;
        for (int j = 0; j < 3; ++j) {
            vals[p][j] = x % 5;
            x /= 5;
        }
    }

    /* Requirement mask for the top row. */
    for (int p = 0; p < NUM_PAT; ++p)
        mask_top[p] = compute_req_mask_top(p);

    /* Requirement mask for a row given its upper neighbour. */
    for (int up = 0; up < NUM_PAT; ++up)
        for (int cur = 0; cur < NUM_PAT; ++cur)
            req_pair[up][cur] = compute_req_mask_with_up(up, cur);

    /* Compute allowed next rows for each (pattern, mask) pair. */
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int m = 0; m < NUM_MASK; ++m) {
            int *list = (int *)calloc(NUM_PAT, sizeof(int));
            int cnt = 0;
            for (int q = 0; q < NUM_PAT; ++q) {
                int ok = 1;
                for (int j = 0; j < 3; ++j) {
                    if ((m >> j) & 1) {                /* required to be >= */
                        if (vals[q][j] < vals[p][j]) {
                            ok = 0;
                            break;
                        }
                    }
                }
                if (ok) list[cnt++] = q;
            }
            next_allowed[p][m] = list;
            next_allowed_cnt[p][m] = cnt;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure tables are initialised once. */
    static int initialized = 0;
    if (!initialized) {
        precompute();
        initialized = 1;
    }

    /* DP arrays for current and next rows. */
    static seq_t cur[NUM_PAT][NUM_MASK];
    static seq_t nxt[NUM_PAT][NUM_MASK];

    /* Initialise cur for the first row. */
    for (int p = 0; p < NUM_PAT; ++p)
        for (int m = 0; m < NUM_MASK; ++m)
            cur[p][m] = 0;
    for (int p = 0; p < NUM_PAT; ++p) {
        int m = mask_top[p];
        if (m != -1)
            cur[p][m] = 1;
    }

    /* Iterate over rows 2..n. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int p = 0; p < NUM_PAT; ++p)
            for (int m = 0; m < NUM_MASK; ++m)
                nxt[p][m] = 0;

        for (int p = 0; p < NUM_PAT; ++p) {
            for (int m = 0; m < NUM_MASK; ++m) {
                seq_t cnt = cur[p][m];
                if (!cnt) continue;
                int cnt_list = next_allowed_cnt[p][m];
                int *list = next_allowed[p][m];
                for (int li = 0; li < cnt_list; ++li) {
                    int q = list[li];
                    int rmask = req_pair[p][q];
                    if (rmask == -1) continue;
                    nxt[q][rmask] += cnt;
                }
            }
        }
        /* Move next to current. */
        for (int p = 0; p < NUM_PAT; ++p)
            for (int m = 0; m < NUM_MASK; ++m)
                cur[p][m] = nxt[p][m];
    }

    /* Sum of valid final rows: requirement mask must be zero. */
    seq_t answer = 0;
    for (int p = 0; p < NUM_PAT; ++p)
        answer += cur[p][0];

    return answer;
}

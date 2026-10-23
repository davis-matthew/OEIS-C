/*
 * A189697  Number of 4Xn binary arrays without the pattern 0 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A189697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 4×n binary arrays that avoid the pattern 0 0 1 in
 *            any vertical, diagonal or antidiagonal position. Computation is
 *            done via dynamic programming on the state of the last two
 *            columns (there are 12 admissible columns, so 144 states in
 *            total)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prepare data for DP: list of admissible 4‑bit patterns            */
static unsigned char  col_bits[16][4];   /* bits in each 4‑bit pattern */
static unsigned char  valid_idx[16];    /* indices of admissible columns */
static unsigned char  valid_cnt;        /* number of admissible columns (12) */

/* DP matrices: state = (prev1, prev2)                                   */
static unsigned char next_count[256];                /* neighbours per state */
static unsigned char next_state[256][16];            /* adjacency list */

/*  DP vectors for a single length                                      */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/*  Cache of sequence values: a_cache[0] = a(1), a_cache[1] = a(2), ...  */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;      /* number of cached terms */

/* --------------------------------------------------------------------- */
static void init_columns(void)
{
    /* build bit tables for all 16 patterns */
    for (int p = 0; p < 16; ++p)
        for (int r = 0; r < 4; ++r)
            col_bits[p][r] = (p >> (3 - r)) & 1;

    /* find admissible columns (no vertical 0‑0‑1) */
    valid_cnt = 0;
    for (int p = 0; p < 16; ++p) {
        bool ok = true;
        for (int r = 0; r < 2; ++r)
            if (col_bits[p][r] == 0 &&
                col_bits[p][r+1] == 0 &&
                col_bits[p][r+2] == 1) {
                ok = false;
                break;
            }
        if (ok) valid_idx[valid_cnt++] = (unsigned char)p;
    }
}

/* --------------------------------------------------------------------- */
static void init_transition(void)
{
    unsigned char sc = valid_cnt;      /* number of admissible columns */
    unsigned char V = sc;
    unsigned char S = sc * sc;         /* number of states */

    /* for each state (v1,v2) compute allowed next columns v3 */
    for (unsigned char s = 0; s < S; ++s) {
        unsigned char v1 = s / sc;     /* column at n-2 */
        unsigned char v2 = s % sc;     /* column at n-1 */
        unsigned int p1 = valid_idx[v1];
        unsigned int p2 = valid_idx[v2];
        next_count[s] = 0;
        for (unsigned char v3 = 0; v3 < V; ++v3) {
            unsigned int p3 = valid_idx[v3];
            bool bad = false;
            for (int r = 0; r < 2; ++r) {
                if (col_bits[p1][r] == 0 &&
                    col_bits[p2][r+1] == 0 &&
                    col_bits[p3][r+2] == 1) { bad = true; break; }
                if (col_bits[p3][r] == 0 &&
                    col_bits[p2][r+1] == 0 &&
                    col_bits[p1][r+2] == 1) { bad = true; break; }
            }
            if (!bad) {
                unsigned short next_s = v2 * sc + v3;
                next_state[s][next_count[s]++] = (unsigned char)next_s;
            }
        }
    }

    /* allocate DP arrays for the states */
    dp_cur  = (seq_t*)calloc(S, sizeof(seq_t));
    dp_next = (seq_t*)calloc(S, sizeof(seq_t));
}

/* --------------------------------------------------------------------- */
static void compute_initial_terms(void)
{
    unsigned char S = valid_cnt * valid_cnt;  /* number of states */

    /* store a(1) and a(2) */
    a_cache = (seq_t*)realloc(a_cache, 19 * sizeof(seq_t));
    a_cache_len = 0;
    seq_t a1 = (seq_t)valid_cnt;
    a_cache[a_cache_len++] = a1;

    seq_t a2 = a1 * a1;             /* all pairs of admissible columns */
    a_cache[a_cache_len++] = a2;

    /* initialise dp_cur for length 2: each pair of columns counts 1 */
    for (unsigned char i = 0; i < S; ++i)
        dp_cur[i] = (seq_t)1;

    /* iterate for lengths 3 … 19 */
    for (idx_t len = 3; len <= 19; ++len) {
        /* zero next DP vector */
        for (unsigned char i = 0; i < S; ++i)
            dp_next[i] = (seq_t)0;

        /* transitions */
        for (unsigned char s = 0; s < S; ++s) {
            seq_t cur = dp_cur[s];
            if (!cur) continue;
            for (size_t m = 0; m < next_count[s]; ++m) {
                unsigned char ns = next_state[s][m];
                dp_next[ns] += cur;
            }
        }

        /* sum all states for a(len) */
        seq_t sum = (seq_t)0;
        for (unsigned char i = 0; i < S; ++i)
            sum += dp_next[i];
        a_cache[a_cache_len++] = sum;

        /* prepare for next iteration */
        seq_t *tmp   = dp_cur;
        dp_cur       = dp_next;
        dp_next      = tmp;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_columns();
        init_transition();
        compute_initial_terms();
        initialized = true;
    }

    if (n == 0) return 0;           /* undefined, safety */

    /* return cached values if available */
    if ((size_t)n <= a_cache_len)
        return a_cache[n-1];

    /* extend DP up to requested n */
    while (a_cache_len < (size_t)n) {
        /* a_next = sum of dp_next over all states */
        /* zero dp_next for next length */
        unsigned char S = valid_cnt * valid_cnt;
        for (unsigned char i = 0; i < S; ++i)
            dp_next[i] = (seq_t)0;

        /* transition one step */
        for (unsigned char s = 0; s < S; ++s) {
            seq_t cur = dp_cur[s];
            if (!cur) continue;
            for (size_t m = 0; m < next_count[s]; ++m) {
                unsigned char ns = next_state[s][m];
                dp_next[ns] += cur;
            }
        }

        /* compute new term a(next_len) */
        seq_t sum = (seq_t)0;
        for (unsigned char i = 0; i < S; ++i)
            sum += dp_next[i];
        a_cache = (seq_t*)realloc(a_cache, (a_cache_len + 1) * sizeof(seq_t));
        a_cache[a_cache_len++] = sum;

        /* swap for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return a_cache[n-1];
}

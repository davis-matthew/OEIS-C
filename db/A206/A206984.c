/*
 * A206984  Number of nX5 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1 in
 *          any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A206984: number of n×5 binary arrays avoiding 010 or 101
 *  in any row, column, main diagonal or anti‑diagonal.
 *
 *  The sequence is computed with a transfer‑matrix DP.  A state is a
 *  pair of consecutive rows (10 bits).  For each state we keep the
 *  number of ways to obtain it.  Transitions are allowed if the new
 *  row together with the two previous rows obeys the forbidden‑pattern
 *  rule in all vertical, diagonal and anti‑diagonal 3‑cell segments.
 *
 *  The DP starts with length 2 (any pair of valid single rows).  For
 *  each subsequent row we apply the pre‑computed transition masks.
 *  The sum of all state values gives a(n).
 *
 *  Complexity:  O(n·2^10)  (≈ n·1024).  Memory:  O(2^10) for DP
 *  plus a small cache of a(n) values.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 5 columns → 32 possible rows */
#define ROWS       5
#define SP         32                /* 2^ROWS  */
#define STATE_COUNT  (SP * SP)        /* pair of rows */
#define MAX_N   100000                /* cache size – enough for all tests */

/* ----------------------------------------------------------------- */
/* Pre‑computation tables                                               */
static bool precomp_done   = false;
static bool row_valid[SP];
static uint32_t allow_mask[SP][SP];

/* Initialise validity of single rows and all transition masks. */
static void init_precomputation(void)
{
    if (precomp_done) return;

    /* Row validity: a row must not contain 010 or 101. */
    for (int r = 0; r < SP; ++r) {
        bool ok = true;
        for (int pos = 0; pos <= ROWS - 3; ++pos) {
            unsigned a = (r >> pos)     & 1u;
            unsigned b = (r >> (pos+1)) & 1u;
            unsigned c = (r >> (pos+2)) & 1u;
            if ((a==0 && b==1 && c==0) || (a==1 && b==0 && c==1)) {
                ok = false; break;
            }
        }
        row_valid[r] = ok;
    }

    /* Transition masks: for every pair (pp,p) of valid rows,
       allow any next row c that keeps all 3‑cell patterns valid. */
    for (int pp = 0; pp < SP; ++pp) {
        if (!row_valid[pp]) continue;
        for (int p = 0; p < SP; ++p) {
            if (!row_valid[p]) continue;
            uint32_t mask = 0;
            for (int c = 0; c < SP; ++c) {
                if (!row_valid[c]) continue;

                bool ok = true;

                /* Vertical 3‑cell check */
                for (int j = 0; j < ROWS; ++j) {
                    unsigned a = (pp >> j) & 1u;
                    unsigned b = (p  >> j) & 1u;
                    unsigned d = (c  >> j) & 1u;
                    if ((a==0 && b==1 && d==0) || (a==1 && b==0 && d==1)) {
                        ok = false; break;
                    }
                }
                if (!ok) continue;

                /* Main diagonal (down‑right) */
                for (int j = 0; j <= ROWS-3; ++j) {
                    unsigned a = (pp >> j)        & 1u;
                    unsigned b = (p  >> (j+1))    & 1u;
                    unsigned d = (c  >> (j+2))    & 1u;
                    if ((a==0 && b==1 && d==0) || (a==1 && b==0 && d==1)) {
                        ok = false; break;
                    }
                }
                if (!ok) continue;

                /* Anti‑diagonal (down‑left) */
                for (int j = ROWS-1; j >= 2; --j) {
                    unsigned a = (pp >> j)        & 1u;
                    unsigned b = (p  >> (j-1))    & 1u;
                    unsigned d = (c  >> (j-2))    & 1u;
                    if ((a==0 && b==1 && d==0) || (a==1 && b==0 && d==1)) {
                        ok = false; break;
                    }
                }
                if (!ok) continue;

                mask |= (1u << c);
            }
            allow_mask[pp][p] = mask;
        }
    }
    precomp_done = true;
}

/* ----------------------------------------------------------------- */
/* DP buffers (two layers).  These are static, so they can hold
   arbitrary‑precision seq_t objects. */
static seq_t dp_buf[2][STATE_COUNT];
static seq_t *dp_curr = dp_buf[0];
static seq_t *dp_next = dp_buf[1];

/* Cache of already computed a(n).  cache[0] is unused. */
static seq_t cache[MAX_N + 1];
static idx_t computed_up_to = 0;        /* highest n that has been cached */

/* Count of valid single rows – needed for a(1) and a(2). */
static seq_t rows_valid_cnt = 0;

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    /* make sure pre‑computation tables exist */
    init_precomputation();

    /* count of valid rows (cached once) */
    if (rows_valid_cnt == 0) {
        seq_t cnt = 0;
        for (int r = 0; r < SP; ++r)
            if (row_valid[r]) ++cnt;
        rows_valid_cnt = cnt;            /* 16 for 5 columns */
    }

    /* initialise the cache for a(1) and a(2) on first use */
    if (computed_up_to == 0) {
        cache[1] = rows_valid_cnt;                     /* 16    */
        cache[2] = rows_valid_cnt * rows_valid_cnt;     /* 256   */
        computed_up_to = 2;
    }

    /* If n already cached, simply return it */
    if (n <= computed_up_to) return cache[n];

    /* Initialise DP for length 2 (only once) – after a(2) is cached */
    static bool dp_initialised = false;
    if (!dp_initialised) {
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_curr[i] = 0;
        for (int pp = 0; pp < SP; ++pp) {
            if (!row_valid[pp]) continue;
            for (int p = 0; p < SP; ++p) {
                if (!row_valid[p]) continue;
                dp_curr[pp*SP + p] = 1;
            }
        }
        dp_initialised = true;
    }

    /* Extend DP step by step up to n */
    for (idx_t step = computed_up_to + 1; step <= n; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        /* perform all transitions */
        for (size_t idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t val = dp_curr[idx];
            if (!val) continue;
            int pp = idx / SP;
            int p  = idx % SP;
            uint32_t mask = allow_mask[pp][p];
            uint32_t m = mask;
            while (m) {
                int c = __builtin_ctz(m);
                dp_next[p*SP + c] += val;
                m &= m - 1;                /* clear lowest set bit */
            }
        }

        /* sum all ways for this length – this is a(step) */
        seq_t sum = 0;
        for (size_t i = 0; i < STATE_COUNT; ++i)
            sum += dp_next[i];
        cache[step] = sum;

        /* prepare for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    computed_up_to = n;
    return cache[n];
}

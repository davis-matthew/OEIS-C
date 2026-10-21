/*
 * A207587  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of n×6 0–1 arrays avoiding horizontal 000 and 001,
 *  and vertical 001 and 111.  The implementation uses a small automaton
 *  of all admissible 6‑bit rows and dynamic programming over them.
 *  It memoises the results, so any call order works.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  seq_t and idx_t are defined by the build system.  */

/*  Boolean flag so that the tables are created only once.  */
static bool initialized = false;

/*  All 6‑bit values that satisfy the horizontal constraint.  */
static unsigned char allowed_rows[64];   /* values of allowed rows          */
static unsigned char allowed_idx[64];    /* mapping from raw row to index   */
static unsigned char allowed_cnt = 0;    /* number of allowed rows          */

/*  Pre‑computed validity of vertical triples: 1 = allowed.  */
static unsigned char vert_ok[64][64][64];

/*  DP tables: dp and nextdp store counts for the current number
 *  of processed rows.  The raw 6‑bit value itself is used as the
 *  index.  */
static seq_t dp[64][64];
static seq_t nextdp[64][64];

/*  Memoised sequence values from a(1) up to the largest n seen.  */
static seq_t *memo = NULL;     /* array of seq_t, indexed by n          */
static idx_t memo_len = 0;     /* number of entries stored (index+1)    */

/*  Helper: initialise tables on first use.  */
static void init(void)
{
    unsigned int r, i, j, k;

    /*  Build the list of admissible rows.  */
    for (r = 0; r < 64; ++r) {
        bool ok = true;
        for (int pos = 0; pos <= 3; ++pos) {
            unsigned int b1 = (r >> pos) & 1u;
            unsigned int b2 = (r >> (pos + 1)) & 1u;
            unsigned int b3 = (r >> (pos + 2)) & 1u;
            if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                (b1 == 0 && b2 == 0 && b3 == 1))
            {
                ok = false;
                break;
            }
        }
        if (ok) {
            allowed_rows[allowed_cnt] = (unsigned char)r;
            allowed_idx[r] = allowed_cnt;
            ++allowed_cnt;
        }
    }

    /*  Pre‑compute vertical constraints for every triple of rows.  */
    for (i = 0; i < 64; ++i) {
        for (j = 0; j < 64; ++j) {
            for (k = 0; k < 64; ++k) {
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    unsigned int x = (i >> col) & 1u;
                    unsigned int y = (j >> col) & 1u;
                    unsigned int z = (k >> col) & 1u;
                    if ((x == 0 && y == 0 && z == 1) ||
                        (x == 1 && y == 1 && z == 1))
                    {
                        ok = false;
                        break;
                    }
                }
                vert_ok[i][j][k] = ok ? 1 : 0;
            }
        }
    }

    /*  Initialise dp for n = 2: every pair of admissible rows is legal.  */
    for (i = 0; i < 64; ++i) {
        for (j = 0; j < 64; ++j) {
            dp[i][j] = 0;
        }
    }
    for (i = 0; i < (unsigned)allowed_cnt; ++i) {
        unsigned int r1 = allowed_rows[i];
        for (j = 0; j < (unsigned)allowed_cnt; ++j) {
            unsigned int r2 = allowed_rows[j];
            dp[r1][r2] = 1;
        }
    }

    /*  Allocate memo array and store a(1) and a(2).  */
    memo_len = 3;                           /* entries for 0,1,2 (0 ignored) */
    memo = (seq_t*)malloc(sizeof(seq_t) * memo_len);
    if (!memo) abort();

    memo[1] = (seq_t)allowed_cnt;                    /* a(1) */
    memo[2] = (seq_t)allowed_cnt * (seq_t)allowed_cnt;  /* a(2) */

    initialized = true;
}

/*  Sum all entries of dp to get the total count for the current number
 *  of rows processed.  */
static seq_t sum_dp(void)
{
    seq_t total = 0;
    for (unsigned int i = 0; i < (unsigned)allowed_cnt; ++i) {
        unsigned int r1 = allowed_rows[i];
        for (unsigned int j = 0; j < (unsigned)allowed_cnt; ++j) {
            unsigned int r2 = allowed_rows[j];
            total += dp[r1][r2];
        }
    }
    return total;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
    }

    if (n == 0) {
        return 0;
    }

    if (n < memo_len) {
        return memo[n];
    }

    /*  Extend memo table up to n.  */
    for (idx_t cur = memo_len; cur <= n; ++cur) {
        /*  Advance DP by one row.  */
        for (unsigned int i = 0; i < 64; ++i) {
            for (unsigned int j = 0; j < 64; ++j) {
                nextdp[i][j] = 0;
            }
        }

        for (unsigned int i = 0; i < (unsigned)allowed_cnt; ++i) {
            unsigned int r1 = allowed_rows[i];
            for (unsigned int j = 0; j < (unsigned)allowed_cnt; ++j) {
                unsigned int r2 = allowed_rows[j];
                seq_t val = dp[r1][r2];
                if (val == 0) continue;
                for (unsigned int k = 0; k < (unsigned)allowed_cnt; ++k) {
                    unsigned int r3 = allowed_rows[k];
                    if (!vert_ok[r1][r2][r3]) continue;
                    nextdp[r2][r3] += val;
                }
            }
        }

        /* Swap dp and nextdp.  */
        for (unsigned int i = 0; i < 64; ++i) {
            for (unsigned int j = 0; j < 64; ++j) {
                dp[i][j] = nextdp[i][j];
            }
        }

        seq_t val = sum_dp();

        /*  Resize memo array if necessary.  */
        if (cur >= memo_len) {
            idx_t old_len = memo_len;
            memo_len = cur + 1;
            seq_t *tmp = (seq_t*)realloc(memo, sizeof(seq_t) * memo_len);
            if (!tmp) abort();
            memo = tmp;
        }

        memo[cur] = val;
    }

    return memo[n];
}

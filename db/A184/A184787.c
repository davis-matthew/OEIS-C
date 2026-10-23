/*
 * A184787  Number of (n+2)X3 0..2 arrays with each 3X3 subblock singular.
 * https://oeis.org/A184787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over the last two rows, counting
   (n+2)×3 0..2 arrays in which every 3×3 subblock is singular. */
 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
 
/* */
 
/* pre‑computed state: is a 3×3 matrix over 0..2 singular? */
static char valid[27][27][27];
/* rows[r] holds the three entries of the r‑th possible row vector (0..2) */
static uint8_t rows[27][3];
/* DP buffers for the pair (prev,last) of rows */
static seq_t *cur = NULL, *next = NULL;
/* memoised sequence values, indexed by n (1‑based) */
static seq_t *cache = NULL;
/* limits of the cache */
static size_t cache_cap = 0, cache_len = 0;
/* number of rows currently processed in the DP (initially 2) */
static idx_t current_rows = 2;
/* initialise everything once */
static int initialized = 0;
 
/* --------------------------------------------------------------- */
static void init_seq(void)
{
    /* encode all 27 combinations of a 3‑vector over 0..2 */
    for (int r = 0; r < 27; ++r) {
        rows[r][0] = r % 3;
        rows[r][1] = (r / 3) % 3;
        rows[r][2] = (r / 9) % 3;
    }
    /* pre‑compute validity of every triple (a,b,c) */
    for (int a = 0; a < 27; ++a)
        for (int b = 0; b < 27; ++b)
            for (int c = 0; c < 27; ++c) {
                int a0 = rows[a][0], a1 = rows[a][1], a2 = rows[a][2];
                int b0 = rows[b][0], b1 = rows[b][1], b2 = rows[b][2];
                int c0 = rows[c][0], c1 = rows[c][1], c2 = rows[c][2];
                int det = a0 * b1 * c2 + a1 * b2 * c0 + a2 * b0 * c1
                        - a2 * b1 * c0 - a1 * b0 * c2 - a0 * b2 * c1;
                valid[a][b][c] = (det == 0);
            }

    /* allocate DP buffers for the 27×27 pairs */
    cur = (seq_t *)malloc(27 * 27 * sizeof(seq_t));
    next = (seq_t *)malloc(27 * 27 * sizeof(seq_t));
    /* initially all pairs of rows are allowed for two rows */
    for (size_t i = 0; i < 27 * 27; ++i)
        cur[i] = 1;

    /* cache up to n = 10000 (guaranteed by the problem limits) */
    cache_cap = 10050;          /* a little safety margin */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    memset(cache, 0, cache_cap * sizeof(seq_t));

    cache_len = 0;              /* no sequence values stored yet */
    current_rows = 2;           /* we have processed two rows so far */
    initialized = 1;
}
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_seq();

    if (n == 0)          /* offset of the sequence is 1 */
        return 0;

    /* need to compute up to rows = n + 2 */
    idx_t target_rows = n + 2;

    if (target_rows > current_rows) {
        /* extend the DP step by step */
        for (idx_t step = current_rows + 1; step <= target_rows; ++step) {
            /* zero the next buffer */
            memset(next, 0, 27 * 27 * sizeof(seq_t));

            /* transition from each (a,b) to (b,c) if (a,b,c) valid */
            for (int a = 0; a < 27; ++a) {
                for (int b = 0; b < 27; ++b) {
                    seq_t val = cur[a * 27 + b];
                    if (val == 0)
                        continue;
                    for (int c = 0; c < 27; ++c) {
                        if (valid[a][b][c])
                            next[b * 27 + c] += val;
                    }
                }
            }

            /* swap buffers: next becomes current */
            seq_t *tmp = cur;
            cur = next;
            next = tmp;

            ++current_rows;          /* we now have processed another row */

            /* count total arrays for the current number of rows */
            seq_t total = 0;
            for (size_t i = 0; i < 27 * 27; ++i)
                total += cur[i];

            /* the sequence index corresponding to this row count */
            idx_t cur_n = current_rows - 2;   /* rows = n+2  =>  n = rows-2 */
            if (cur_n >= 1 && cur_n > cache_len && (size_t)cur_n <= cache_cap) {
                cache[cur_n - 1] = total;
                cache_len = cur_n;
            }
        }
    }

    /* cache holds values starting at index 0 for n=1 */
    return cache[n - 1];
}

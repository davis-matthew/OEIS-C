/*
 * A210384  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          one, two or three distinct clockwise edge differences.
 * https://oeis.org/A210384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula: sequence defined by a recurrence.  
   a(1) = 597, a(2) = 13399, a(3) = 300653, ...
   We compute a(n) as the number of (n+1)×3 arrays over {0,1,2}
   where every 2×2 subblock has 1, 2, or 3 distinct clockwise
   edge differences.  The algorithm uses transfer‑matrix
   dynamic programming over the 3³=27 possible rows.
   Complexities: O(n·27²) time, O(27) memory.
   The implementation stores a cache so calls with any order of
   n are O(1) after the first incremental computation.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 27   /* number of possible rows of length 3 */

/* compatibility matrix: M[a][b] == true if a can be followed by b */
static bool M[ROWS][ROWS];
/* row representation in base 3: rowv[i][j] is the j‑th digit of row i */
static int rowv[ROWS][3];

/* initialisation of rows and compatibility matrix, run once */
static void init_once(void)
{
    /* fill 'rowv' with base‑3 digits of each row index */
    for (int idx = 0; idx < ROWS; ++idx) {
        int val = idx;
        for (int k = 2; k >= 0; --k) {
            rowv[idx][k] = val % 3;
            val /= 3;
        }
    }

    /* precompute compatibility */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            bool ok = true;
            for (int col = 0; col < 2; ++col) {  /* two 2×2 blocks per pair of rows */
                int diffs[4];
                diffs[0] = rowv[a][col + 1] - rowv[a][col];          /* top edge   */
                diffs[1] = rowv[b][col + 1] - rowv[a][col + 1];      /* right edge */
                diffs[2] = rowv[b][col] - rowv[b][col + 1];          /* bottom edge*/
                diffs[3] = rowv[a][col] - rowv[b][col];              /* left edge */

                /* count distinct differences */
                int cnt = 1;
                for (int i = 1; i < 4; ++i) {
                    bool seen = false;
                    for (int j = 0; j < i; ++j)
                        if (diffs[i] == diffs[j]) { seen = true; break; }
                    if (!seen) cnt++;
                }
                if (cnt > 3) { ok = false; break; }    /* four distinct not allowed */
            }
            M[a][b] = ok;
        }
    }
}

/* public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* cache[1]..cache[max] */
    static size_t cache_cap = 0, cache_len = 0;  /* cache_len = highest computed n */
    static bool init = false;
    static seq_t *cur = NULL, *nxt = NULL;  /* dynamic programming vectors */

    /* first call: initialise everything */
    if (!init) {
        init_once();
        cur = (seq_t*)malloc(ROWS * sizeof(seq_t));
        nxt = (seq_t*)malloc(ROWS * sizeof(seq_t));
        for (int i = 0; i < ROWS; ++i) cur[i] = (seq_t)1;   /* one way to form a single row */

        cache_cap = n > 256 ? n : 256;   /* initial capacity */
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;             /* unused, keep zeroed */
        cache_len = 0;
        init = true;
    }

    /* grow cache storage if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to requested n */
    while (cache_len < n) {
        /* zero next vector */
        for (int j = 0; j < ROWS; ++j) nxt[j] = (seq_t)0;

        /* transition */
        for (int i = 0; i < ROWS; ++i) {
            if (cur[i] == (seq_t)0) continue;
            for (int j = 0; j < ROWS; ++j) {
                if (!M[i][j]) continue;
                nxt[j] += cur[i];
            }
        }

        /* swap current and next */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;

        /* sum of all states gives a(cache_len+1) */
        seq_t sum = (seq_t)0;
        for (int k = 0; k < ROWS; ++k) sum += cur[k];
        cache[++cache_len] = sum;
    }

    return cache[n];
}

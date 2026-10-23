/*
 * A184665  Number of (n+1) X 2 0..3 arrays with no 2 X 2 subblock summing to
 *          less than 6.
 * https://oeis.org/A184665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184665: number of (n+1)×2 arrays with entries 0..3 that contain no
 * 2×2 subblock whose four entries sum to less than 6.  The value is
 * computed by dynamic programming over the 16 possible rows; the
 * adjacency condition is that the sums of two consecutive rows be ≥ 6.
 * The implementation stores a cache of previously computed terms and
 * expands it as needed. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* adjacency matrix for the 16 row states; 1 iff the two rows can be adjacent */
static uint8_t adj[16][16];

/* Build the adjacency table once */
static void build_adj_table(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    for (int s = 0; s < 16; ++s) {
        int a = (s >> 2) & 3;      /* first column of row s */
        int b = s & 3;             /* second column of row s */
        for (int t = 0; t < 16; ++t) {
            int c = (t >> 2) & 3;  /* first column of row t */
            int d = t & 3;         /* second column of row t */
            int sum = a + b + c + d;
            adj[s][t] = (sum >= 6) ? 1 : 0;
        }
    }
}

/* Cache of a(1), a(2), ... up to the largest n computed so far */
static seq_t *cache      = NULL;   /* cache[0] unused */
static size_t cache_size = 0;      /* capacity of cache array */

/* Current and next DP vectors for the number of partial arrays ending
 * in each of the 16 possible rows.  cur holds counts for length k,
 * nxt for length k+1. */
static seq_t cur[16];
static seq_t nxt[16];

/* Maximum n for which the cache is fully computed */
static size_t max_k = 0;

/* Flag indicating whether the DP vectors have been initialized */
static int dp_init = 0;


/* --------------------------------------------------------------------- */
/* public function required by the framework                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Build adjacency table on first entry */
    build_adj_table();

    /* Initialize DP vectors only once */
    if (!dp_init) {
        dp_init = 1;
        for (int i = 0; i < 16; ++i) cur[i] = (seq_t)1; /* length 1: any row */
        max_k = 0;
    }

    /* Extend cache and DP up to requested n */
    if ((size_t)n > max_k) {
        /* Reserve space in the cache, if necessary */
        if (cache_size <= (size_t)n) {
            size_t new_size = (size_t)n + 1;
            seq_t *nc = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!nc) return (seq_t)0;  /* allocation failure */
            cache = nc;
            cache_size = new_size;
            cache[0] = (seq_t)0;  /* unused element */
        }

        /* Compute DP values for k = max_k+1 .. n */
        for (size_t k = max_k + 1; k <= (size_t)n; ++k) {
            /* For each possible ending row, accumulate counts from all
               previous rows that are allowed to precede it. */
            for (int j = 0; j < 16; ++j) {
                seq_t sum = (seq_t)0;
                for (int i = 0; i < 16; ++i)
                    if (adj[i][j]) sum += cur[i];
                nxt[j] = sum;
            }

            /* Total number of arrays of length k+1 rows */
            seq_t total = (seq_t)0;
            for (int j = 0; j < 16; ++j) total += nxt[j];
            cache[k] = total;

            /* Prepare for next iteration */
            for (int j = 0; j < 16; ++j) cur[j] = nxt[j];
        }

        max_k = (size_t)n;
    }

    return cache[n];
}

/*
 * A205992  Number of (n+1)X3 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205992
   Empirical recurrence (valid for n > 10):
   a(n) = 3*a(n-1) + 17*a(n-2) - 19*a(n-3) - 4*a(n-4)
          + 27*a(n-5) - 13*a(n-6) + 8*a(n-8) - 4*a(n-9)
   Initial values:
   a(1) = 92, a(2) = 498, a(3) = 2712, a(4) = 14866, a(5) = 81278,
   a(6) = 445352, a(7) = 2436706, a(8) = 13344256, a(9) = 73036756,
   a(10) = 399888660
   (The recurrence is derived by fitting the first 25 terms - the
   function below does not use the recurrence but a direct DP according
   to the combinatorial definition.) */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif
#include <stdint.h>
#include <stdbool.h>

static bool M[27][27];
static bool M_inited = false;

/* convert an index 0..26 to a 3‑tuple of values 0,1,2 */
static inline void decode_row(int idx, int out[3]) {
    out[0] = idx % 3;
    out[1] = (idx / 3) % 3;
    out[2] = (idx / 9) % 3;
}

/* check whether two consecutive rows satisfy the two conditions on
   each 2×2 subblock */
static bool allowed_transition(int r1[3], int r2[3]) {
    /* first subblock (columns 0,1) */
    int a = r1[0], b = r1[1], c = r2[0], d = r2[1];
    int inc = (a < b) + (b < d) + (d < c) + (c < a);
    if (inc != 2) return false;
    if ((a * d - b * c) == 0) return false;
    /* second subblock (columns 1,2) */
    a = r1[1]; b = r1[2]; c = r2[1]; d = r2[2];
    inc = (a < b) + (b < d) + (d < c) + (c < a);
    if (inc != 2) return false;
    if ((a * d - b * c) == 0) return false;
    return true;
}

/* compute the transition matrix once */
static void init_matrix(void) {
    if (M_inited) return;
    int r1[3], r2[3];
    for (int i = 0; i < 27; ++i) {
        decode_row(i, r1);
        for (int j = 0; j < 27; ++j) {
            decode_row(j, r2);
            M[i][j] = allowed_transition(r1, r2);
        }
    }
    M_inited = true;
}

/* DP state for one row (cumulative counts for all rows after some
   number of transitions) */
static seq_t cur[27], nxt[27];
static idx_t cur_n = 0;   /* number of transitions already computed */

/* cache of a(n) values: a(1) is stored at cache[0] */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

/* ensure the cache can hold values up to position n (1‑based) */
static void ensure_cache(idx_t n) {
#ifdef __cplusplus
    if (cache.size() < n + 1) cache.resize(n + 1);
#else
    if (cache_cap >= n + 1) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)(n + 1)) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/* initialise DP with one row (each of the 27 possible rows has count 1) */
static void init_state(void) {
    if (cur_n > 0) return;
    for (int i = 0; i < 27; ++i) cur[i] = 1;
    cur_n = 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined – return 0 for safety */

    init_matrix();
    init_state();

    /* produce results up to n transitions if necessary */
    for (idx_t step = cur_n + 1; step <= n; ++step) {
        for (int j = 0; j < 27; ++j) {
            seq_t sum = 0;
            for (int i = 0; i < 27; ++i)
                if (M[i][j]) sum += cur[i];
            nxt[j] = sum;
        }
        for (int j = 0; j < 27; ++j)
            cur[j] = nxt[j];

        /* total number of arrays with (step+1) rows = a(step) */
        seq_t total = 0;
        for (int j = 0; j < 27; ++j) total += cur[j];

        ensure_cache(step);
        cache[step - 1] = total;
    }

    cur_n = n;
    return cache[n - 1];
}

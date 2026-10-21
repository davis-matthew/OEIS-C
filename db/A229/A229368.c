/*
 * A229368  Number of nX4 0..2 arrays avoiding 11 horizontally, 22 vertically
 *          and 00 antidiagonally.
 * https://oeis.org/A229368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Poster: OEIS A229368
   Formula (computed by transfer‐matrix DP):
   a(1) = 60
   For n > 1, a(n) = sum over all valid 4‑row patterns s of number of walks of length n
   in the directed graph whose vertices are the 60 patterns with no horizontal 11,
   and edges represent allowed vertical 22 and anti‑diagonal 00 constraints. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_STATES 81

/* state[i][c] holds the value of column c (0‑based) for pattern i */
static uint8_t state[MAX_STATES][4];

/* trans[i][j] == 1 iff pattern i can be followed by pattern j */
static unsigned char trans[MAX_STATES][MAX_STATES]; /* 0/1 */

/* DP arrays (static, size known at compile time) */
static seq_t dp_prev[MAX_STATES];
static seq_t dp_cur[MAX_STATES];

/* Cache of a(n) values */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0;      /* number of slots currently allocated */
static idx_t computed_max = 0;    /* largest n for which a_cache[n] is valid */

/* Number of valid patterns */
static idx_t nstates = 0;

/* One‑time initialization flag */
static int initialized = 0;

/* Small helper for realloc in C++ */
#ifdef __cplusplus
static seq_t *cxx_realloc(seq_t *old, idx_t new_sz) {
    seq_t *newp = new seq_t[new_sz];
    for (idx_t i = 0; i < new_sz && old && i < cache_size; ++i)
        newp[i] = old[i];
    delete[] old;
    return newp;
}
#endif

/* Internal initialization routine */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* Enumerate all 3^4 = 81 patterns and keep only those with no
       horizontal occurrence of \"11\". */
    nstates = 0;
    for (idx_t num = 0; num < 81; ++num) {
        uint8_t dig[4];
        idx_t tmp = num;
        for (int pos = 0; pos < 4; ++pos) {
            dig[pos] = tmp % 3;
            tmp /= 3;
        }
        int ok = 1;
        for (int i = 0; i < 3; ++i)
            if (dig[i] == 1 && dig[i+1] == 1) { ok = 0; break; }
        if (ok) {
            for (int i = 0; i < 4; ++i) state[nstates][i] = dig[i];
            ++nstates;
        }
    }

    /* Build transition matrix */
    for (size_t i = 0; i < nstates; ++i) {
        for (size_t j = 0; j < nstates; ++j) {
            int ok = 1;
            /* vertical 22 */
            for (int c = 0; c < 4; ++c)
                if (state[i][c] == 2 && state[j][c] == 2) { ok = 0; break; }
            /* anti‑diagonal 00 */
            if (ok) {
                for (int k = 0; k < 3; ++k)
                    if (state[i][k+1] == 0 && state[j][k] == 0) { ok = 0; break; }
            }
            trans[i][j] = ok;
        }
    }

    /* Initialize dp_prev for n = 1 (each state has 1 sequence) */
    for (size_t i = 0; i < nstates; ++i) dp_prev[i] = (seq_t)1;

    /* Allocate cache for a(0) and a(1) */
    cache_size = 2;
    computed_max = 1;
    a_cache = NULL;
#ifdef __cplusplus
    a_cache = new seq_t[cache_size];
#else
    a_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
#endif
    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)nstates;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init();

    /* Return cached value if already computed */
    if (n <= computed_max) return a_cache[n];

    /* Extend computation up to n */
    for (idx_t step = computed_max + 1; step <= n; ++step) {
        /* Zero dp_cur */
        for (size_t i = 0; i < nstates; ++i) dp_cur[i] = (seq_t)0;

        /* Transition step */
        for (size_t from = 0; from < nstates; ++from) {
            seq_t val = dp_prev[from];
            if (val == (seq_t)0) continue;
            for (size_t to = 0; to < nstates; ++to)
                if (trans[from][to]) dp_cur[to] += val;
        }

        /* Sum all paths of length step */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < nstates; ++i) sum += dp_cur[i];

        /* Grow cache if necessary */
        if (step >= (idx_t)cache_size) {
            idx_t new_sz = cache_size * 2;
            if (new_sz <= step) new_sz = step + 1;
#ifdef __cplusplus
            a_cache = cxx_realloc(a_cache, new_sz);
#else
            seq_t *tmp = (seq_t*)realloc(a_cache, new_sz * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
#endif
            cache_size = new_sz;
        }
        a_cache[step] = sum;

        /* Prepare dp_prev for next step */
        for (size_t i = 0; i < nstates; ++i) dp_prev[i] = dp_cur[i];
    }

    computed_max = n;
    return a_cache[n];
}

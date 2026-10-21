/*
 * A224372  Number of nX6 0..2 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) -283*a(n-2) +1386*a(n-3) -3562*a(n-4)
 *            +4940*a(n-5) * -3840*a(n-6) -14256*a(n-7) +50007*a(n-8)
 *            -26386*a(n-9) +141154*a(n-10) * +250789*a(n-11) +41356*a(n-12)
 *            +274344*a(n-13) +134400*a(n-14) for n>18 * * Initial terms (a(1)
 *            ... a(18) are computed by a DP over unimodal rows): * a(1)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Types provided by the build, do not define them here.           */
/*  typedef uint64_t idx_t;                                           */
/*  typedef unsigned __int128 seq_t;      (or other arith type)       */
/* ------------------------------------------------------------------ */

/*---------------------------------------------------------------------*/
/* Representation of a single 6‑row (unimodal)                       */
/*---------------------------------------------------------------------*/
struct Row { unsigned char v[6]; };

/*---------------------------------------------------------------------*/
/* Global state: arrays of rows, transitions, memoised a(n)          */
/*---------------------------------------------------------------------*/
static struct Row *rows = NULL;          /* list of unimodal rows   */
static idx_t R = 0;                      /* number of rows          */
static unsigned char *allow = NULL;      /* adjacency matrix       */
static seq_t *memo = NULL;               /* memoised a(n)           */
static idx_t memo_size = 0;              /* largest n computed      */

/*---------------------------------------------------------------------*/
/* Coefficients of the 14‑term linear recurrence                      */
/*---------------------------------------------------------------------*/
static const int coeffs[14] = {
    28, -283, 1386, -3562, 4940, -3840,
    -14256, 50007, -26386, 141154, 250789,
    41356, 274344, 134400
};

/*---------------------------------------------------------------------*/
/* Test whether a 6‑tuple over 0..2 is unimodal (non‑decreasing then
 * non‑increasing).                                                    */
/*---------------------------------------------------------------------*/
static bool is_unimodal(const unsigned char *a)
{
    int i = 0;
    while (i + 1 < 6 && a[i] <= a[i + 1]) ++i;
    for (int j = i; j + 1 < 6; ++j)
        if (a[j] < a[j + 1]) return false;
    return true;
}

/*---------------------------------------------------------------------*/
/* Build all unimodal rows and the adjacency matrix                 */
/*---------------------------------------------------------------------*/
static void init_rows_and_transitions(void)
{
    if (rows) return;                       /* already built */

    /* Enumerate the 3^6 = 729 possible rows. */
    int total_combos = 1;
    for (int k = 0; k < 6; ++k) total_combos *= 3;

    struct Row *tmp = (struct Row *)malloc(total_combos * sizeof(struct Row));
    int count = 0;
    unsigned char val[6];

    for (int num = 0; num < total_combos; ++num) {
        int x = num;
        for (int j = 0; j < 6; ++j) {
            val[j] = (unsigned char)(x % 3);
            x /= 3;
        }
        if (is_unimodal(val)) {
            for (int j = 0; j < 6; ++j) tmp[count].v[j] = val[j];
            ++count;
        }
    }
    rows = tmp;
    R = (idx_t)count;

    /* Build adjacency: row i can precede row j iff
     * for all k=1..5, rows[j].v[k-1] >= rows[i].v[k]   (antidiagonals nondecreasing) */
    allow = (unsigned char *)calloc((size_t)R * (size_t)R, 1);
    for (idx_t i = 0; i < R; ++i) {
        for (idx_t j = 0; j < R; ++j) {
            bool ok = true;
            for (int k = 1; k < 6 && ok; ++k)
                if (rows[j].v[k - 1] < rows[i].v[k]) ok = false;
            allow[i * R + j] = (unsigned char)ok;
        }
    }
}

/*---------------------------------------------------------------------*/
/* Compute a(1) … a(18) by dynamic programming (DP over rows).        */
/*---------------------------------------------------------------------*/
static void compute_small_terms(void)
{
    seq_t *dp_prev = (seq_t *)calloc((size_t)R, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc((size_t)R, sizeof(seq_t));

    /* Each row may start a 1‑length array. */
    for (idx_t i = 0; i < R; ++i) dp_prev[i] = (seq_t)1;
    memo[1] = (seq_t)R;       /* sum over all starting rows */

    for (idx_t row = 2; row <= 18; ++row) {
        for (idx_t j = 0; j < R; ++j) dp_next[j] = 0;
        for (idx_t i = 0; i < R; ++i) {
            seq_t val = dp_prev[i];
            if (!val) continue;
            for (idx_t j = 0; j < R; ++j)
                if (allow[i * R + j]) dp_next[j] += val;
        }
        seq_t sum = 0;
        for (idx_t j = 0; j < R; ++j) sum += dp_next[j];
        memo[row] = sum;

        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    free(dp_prev);
    free(dp_next);
    memo_size = 18;
}

/*---------------------------------------------------------------------*/
/* Extend memoised values up to at least n (using the recurrence).   */
/*---------------------------------------------------------------------*/
static void enlarge_memo(idx_t n)
{
    if (n <= memo_size) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) exit(1);   /* allocation failed */
    memo = new_mem;

    for (idx_t idx = memo_size + 1; idx <= n; ++idx) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 14; ++k) {
            idx_t src = idx - 1 - k;
            seq_t term = memo[src];
            seq_t prod = term * (seq_t) ( (coeffs[k] >= 0) ? coeffs[k] : -coeffs[k] );
            if (coeffs[k] >= 0) pos += prod;
            else neg += prod;
        }
        memo[idx] = pos - neg;   /* result is non‑negative */
    }
    memo_size = n;
}

/*---------------------------------------------------------------------*/
/* Public function required by the problem.                           */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        init_rows_and_transitions();
        memo = (seq_t *)calloc((size_t)19, sizeof(seq_t)); /* indices 0..18, use 1..18 */
        compute_small_terms();
    }

    enlarge_memo(n);
    return memo[n];
}

/*
 * A145870  Number of involutions of length 2n which are invariant under the
 *          reverse-complement map and have no decreasing subsequences of
 *          length 8.
 * https://oeis.org/A145870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} C(n,j) * Catalan(floor((j+1)/2)) *
 *            Catalan(ceil((j+1)/2)) * Motzkin(n-j)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computational helpers for Catalan and Motzkin numbers cached
 * across calls.  They are recomputed only when a larger index is
 * required. */
#include <stdlib.h>

static seq_t *catalan_cache = NULL;
static size_t catalan_cap   = 0;   /* number of stored Catalan values */

static seq_t *motzkin_cache = NULL;
static size_t motzkin_cap   = 0;   /* number of stored Motzkin values */

/* Ensure Catalan numbers up to 'needed' (inclusive) are available. */
static void ensure_catalan(idx_t needed)
{
    if (needed + 1 <= catalan_cap) return;          /* already sufficient */

    size_t new_cap = (size_t)(needed + 1);
    seq_t *new_arr = (seq_t *)realloc(catalan_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;                          /* allocation failed */

    /* Initialise from scratch if empty. */
    if (catalan_cap == 0) {
        new_arr[0] = 1;
        catalan_cap = 1;
    }

    /* Compute Catalan numbers from current size up to 'needed'. */
    for (idx_t i = catalan_cap; i <= needed; ++i) {
        /* Recurrence: C_i = C_{i-1} * 2*(2*i-1)/(i+1)   (i >= 1) */
        seq_t prev  = new_arr[i-1];
        seq_t mult  = (seq_t)(2 * (2 * i - 1));
        seq_t denom = (seq_t)(i + 1);
        new_arr[i] = prev * mult / denom;
    }

    catalan_cache = new_arr;
    catalan_cap   = new_cap;
}

/* Ensure Motzkin numbers up to 'needed' (inclusive) are available. */
static void ensure_motzkin(idx_t needed)
{
    if (needed + 1 <= motzkin_cap) return;          /* already sufficient */

    size_t new_cap = (size_t)(needed + 1);
    seq_t *new_arr = (seq_t *)realloc(motzkin_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;                          /* allocation failed */

    /* Initialise from scratch if empty. */
    if (motzkin_cap == 0) {
        new_arr[0] = 1;
        if (new_cap > 1) new_arr[1] = 1;
        motzkin_cap = new_cap;
    } else {
        /* Extend existing array: at least indices 0 and 1 must exist. */
        if (motzkin_cap == 1 && new_cap > 1) new_arr[1] = 1;
    }

    /* Compute Motzkin numbers from current size up to 'needed'. */
    for (idx_t i = motzkin_cap; i <= needed; ++i) {
        if (i < 2) continue;   /* M_0 and M_1 already set */
        seq_t term1 = new_arr[i-1] * (seq_t)(2 * i + 1);
        seq_t term2 = new_arr[i-2] * (seq_t)(3 * i - 3);
        seq_t num   = term1 + term2;
        seq_t denom = (seq_t)(i + 2);
        new_arr[i] = num / denom;
    }

    motzkin_cache = new_arr;
    motzkin_cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n can be zero; compute required maximum indices. */
    idx_t max_catalan = (n + 2) / 2;          /* ceil((n+1)/2) */
    ensure_catalan(max_catalan);
    ensure_motzkin(n);

    seq_t sum   = 0;
    seq_t binom = 1;                          /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        idx_t a1 = (j + 1) / 2;               /* floor((j+1)/2) */
        idx_t a2 = (j + 2) / 2;               /* ceil((j+1)/2)  */

        seq_t cat1 = catalan_cache[a1];
        seq_t cat2 = catalan_cache[a2];
        seq_t motz = motzkin_cache[n - j];

        seq_t term = binom;
        term *= cat1;
        term *= cat2;
        term *= motz;

        sum += term;

        /* Update binomial coefficient for next j. */
        if (j < n) {
            binom = (binom * (n - j)) / (j + 1);
        }
    }
    return sum;
}

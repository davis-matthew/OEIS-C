/*
 * A125335  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base 25 circular n‑digit numbers with adjacent
 *            digits differing by 3 or less. a(0) = 1. For n ≥ 1, a(n) =
 *            trace(A^n) where A is the 25×25 adjacency matrix having 1 iff
 *            |i−j| ≤ 3. The sequence satisfies a linear recurrence of order
 *            25 derived from the characteristic polynomial of...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t coeff[26];
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t init_done = 0;

/* Multiply two 25×25 matrices.  The result is written into out. */
static void mul_mat(seq_t out[25][25], const seq_t a[25][25], const seq_t b[25][25])
{
    seq_t tmp[25][25];
    for (idx_t i = 0; i < 25; ++i) {
        for (idx_t j = 0; j < 25; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 25; ++k)
                sum += a[i][k] * b[k][j];
            tmp[i][j] = sum;
        }
    }
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j)
            out[i][j] = tmp[i][j];
}

/* Initialise the first 25 terms and the recurrence coefficients. */
static void init(void)
{
    if (init_done) return;
    init_done = 1;

    /* Base adjacency matrix A. */
    seq_t A[25][25];
    for (idx_t i = 0; i < 25; ++i) {
        for (idx_t j = 0; j < 25; ++j) {
            int diff = (i > j) ? (int)(i - j) : (int)(j - i);
            A[i][j] = (seq_t)(diff <= 3 ? 1 : 0);
        }
    }

    /* Compute a(1)…a(25) as traces of A^k. */
    seq_t *first = (seq_t*)malloc((25 + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= 25; ++i)
        first[i] = 0;

    seq_t cur[25][25];
    for (idx_t i = 0; i < 25; ++i)
        for (idx_t j = 0; j < 25; ++j)
            cur[i][j] = A[i][j];

    /* Trace of A^1 */
    for (idx_t i = 0; i < 25; ++i) first[1] += cur[i][i];

    /* Subsequent powers */
    for (idx_t k = 2; k <= 25; ++k) {
        mul_mat(cur, cur, A);          /* cur = cur * A  */
        for (idx_t i = 0; i < 25; ++i)
            first[k] += cur[i][i];
    }

    /* Allocate cache for 0…25 and store initial terms. */
    cache_cap = 26;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = 1;
    for (idx_t i = 1; i <= 25; ++i)
        cache[i] = first[i];

    /* Compute the recurrence coefficients using Newton’s identities. */
    for (idx_t m = 1; m <= 25; ++m) {
        seq_t num = first[m];
        for (idx_t k = 1; k < m; ++k)
            num += coeff[k] * first[m - k];
        coeff[m] = -num / (seq_t)m;
    }

    free(first);
}

/* Grow cache up to index n, filling new entries by the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t newcap = n + 1;
    seq_t *newptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i <= 25) continue;                /* already known */
        seq_t val = 0;
        for (idx_t k = 1; k <= 25; ++k)
            val += cache[i - k] * coeff[k];
        cache[i] = -val;                      /* a(i) = -sum of coeff•previous */
    }
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (!init_done) init();
    ensure_cache(n);
    return cache[n];
}

/*
 * A015169  Sum of (Gaussian) q-binomial coefficients for q=-6.
 * https://oeis.org/A015169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0}^n [n k]_{q} with q = –6 (Gaussian q‑binomial sum)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* idx_t and seq_t are defined by the build environment */
static seq_t *row = NULL;       /* Gaussian q‑binomial row for current n */
static seq_t *sum_cache = NULL; /* cached a(n) values */
static seq_t *pow_q = NULL;     /* powers of q = –6 */
static idx_t row_cap = 0, sum_cap = 0, pow_cap = 0;
static idx_t max_n_computed = 0; /* highest n for which sum_cache has been filled */

/* Ensure pow_q[i] = q^i for all i ≤ n. */
static void ensure_pow_q(idx_t n)
{
    if (pow_cap > n) return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(pow_q, new_cap * sizeof(seq_t));
    if (!tmp) return;
    if (!pow_q) {                /* first allocation */
        tmp[0] = 1;
        pow_cap = 1;
    }
    for (idx_t i = pow_cap; i <= n; ++i)
        tmp[i] = tmp[i - 1] * (seq_t)-6;
    pow_q = tmp;
    pow_cap = new_cap;
}

/* Ensure row, sum_cache, and pow_q have capacity at least n+1. */
static void ensure_cap(idx_t n)
{
    if (row_cap > n && sum_cap > n) return;
    idx_t new_cap = n + 1;
    if (row_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(row, new_cap * sizeof(seq_t));
        if (!tmp) return;
        if (!row) {
            memset(tmp, 0, new_cap * sizeof(seq_t));
        } else {
            memset(tmp + row_cap, 0, (new_cap - row_cap) * sizeof(seq_t));
        }
        row = tmp;
        row_cap = new_cap;
    }
    if (sum_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;
        if (!sum_cache) {
            memset(tmp, 0, new_cap * sizeof(seq_t));
        } else {
            memset(tmp + sum_cap, 0, (new_cap - sum_cap) * sizeof(seq_t));
        }
        sum_cache = tmp;
        sum_cap = new_cap;
    }
}

/* Initialise the cache for n=0. */
static void init_base(void)
{
    if (!row || !sum_cache || !pow_q) {
        row = (seq_t *)calloc(1, sizeof(seq_t));
        sum_cache = (seq_t *)calloc(1, sizeof(seq_t));
        pow_q = (seq_t *)calloc(1, sizeof(seq_t));
        if (!row || !sum_cache || !pow_q) return;
        row[0] = 1;
        sum_cache[0] = 1;
        pow_q[0] = 1;
        row_cap = sum_cap = pow_cap = 1;
        max_n_computed = 0;
    }
}

seq_t a(idx_t n)
{
    if (!row || !sum_cache || !pow_q) init_base();
    if (n == 0) return sum_cache[0];
    if (n <= max_n_computed) return sum_cache[n];

    ensure_pow_q(n);
    ensure_cap(n);

    for (idx_t cur = max_n_computed + 1; cur <= n; ++cur) {
        if (row[cur] != 0) row[cur] = 0;          /* guarantee zero before use */
        for (idx_t k = cur; k > 0; --k) {
            seq_t term = pow_q[cur - k] * row[k - 1];
            row[k] = row[k] + term;
        }
        row[0] = 1; /* first element is always 1 */

        seq_t s = row[0];
        for (idx_t k = 1; k <= cur; ++k)
            s += row[k];
        sum_cache[cur] = s;
    }
    max_n_computed = n;
    return sum_cache[n];
}

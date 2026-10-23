/*
 * A144015  Expansion of e.g.f. 1/(1 - sin(4*x))^(1/4).
 * https://oeis.org/A144015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Expansion of e.g.f. 1/(1 - sin(4*x))^(1/4). Recurrence
 *            derived from A'(x) = A(x)^3 / A(-x)^2: a(0) = 1, a(1) = 1 for n
 *            ≥ 1: a(n+1) = c3_n + Σ_{k=0}^{n-1} (-1)^{n-k} binom(n,k) a(k+1)
 *            conv2_{n-k} where conv2_m = Σ_{i=0}^{m} binom(m,i) a(i) a(m-i)
 *            c3_n = Σ_{k=0}^{n} binom(n,k) conv2_k a(n-k)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t */
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------------------- */
static seq_t *a_vals      = NULL;   /* a_vals[i] = a(i)               */
static seq_t *conv2_vals  = NULL;   /* conv2_vals[i] = conv2(i)       */
static size_t capacity   = 0;       /* current allocation size       */
static size_t computed_n = 0;       /* highest index computed so far */

/* ------------------------------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (needed + 1 <= capacity) return;
    size_t new_cap = needed + 1;
    a_vals     = seq_realloc(a_vals,   capacity, new_cap);
    conv2_vals = seq_realloc(conv2_vals, capacity, new_cap);
    capacity = new_cap;
}

/* ------------------------------------------------------------------------- */
static seq_t binom_unsigned(idx_t n, idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (n - k + i) / i;   /* r = C(n,i)  */
    }
    return r;
}

/* ------------------------------------------------------------------------- */
static void compute_conv2_at(idx_t m)
{
    seq_t sum = 0;
    seq_t bin = 1;
    for (idx_t i = 0; i <= m; ++i)
    {
        if (i > 0) bin = bin * (m - i + 1) / i;   /* update to C(m,i) */
        sum += bin * a_vals[i] * a_vals[m - i];
    }
    conv2_vals[m] = sum;
}

/* ------------------------------------------------------------------------- */
static seq_t compute_convolution3(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0) bin = bin * (n - k + 1) / k;   /* C(n,k) */
        sum += bin * conv2_vals[k] * a_vals[n - k];
    }
    return sum;
}

/* ------------------------------------------------------------------------- */
static seq_t sum_pos_neg(idx_t n, seq_t *sum_pos, seq_t *sum_neg)
{
    seq_t bin = 1;
    *sum_pos = 0;
    *sum_neg = 0;
    for (idx_t k = 0; k < n; ++k)
    {
        if (k > 0) bin = bin * (n - k + 1) / k;   /* C(n,k) */
        seq_t term = bin * a_vals[k + 1] * conv2_vals[n - k];
        if (((n - k) & 1) == 0)
            *sum_pos += term;
        else
            *sum_neg += term;
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* First-time initialization */
    if (capacity == 0)
    {
        ensure_capacity(10);         /* allocate some initial space   */
        a_vals[0] = 1;
        a_vals[1] = 1;
        compute_conv2_at(0);         /* conv2_0 = 1 */
        compute_conv2_at(1);         /* conv2_1 = 2  */
        computed_n = 1;
    }

    if (n <= computed_n) return a_vals[n];

    ensure_capacity(n);

    for (size_t cur = computed_n; cur <= (size_t)n - 1; ++cur)
    {
        /* conv3 for current index cur */
        seq_t conv3_cur = compute_convolution3((idx_t)cur);

        /* sum_pos and sum_neg for a[cur+1] */
        seq_t sum_p = 0, sum_n = 0;
        sum_pos_neg((idx_t)cur, &sum_p, &sum_n);

        /* a[cur+1] = conv3 + sum_n - sum_p */
        seq_t tmp = conv3_cur + sum_n;
        a_vals[cur + 1] = tmp - sum_p;

        /* compute conv2 for new index cur+1 */
        compute_conv2_at((idx_t)(cur + 1));
    }

    computed_n = (size_t)n;
    return a_vals[n];
}

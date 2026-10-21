/*
 * A062245  Expansion of Hauptmodul for group G'_{27|3}.
 * https://oeis.org/A062245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the Euler transform of the period‑36 sequence * c(k) =
 *            [1, -2, 1, -1, 1, -2, 1, -1, 0, -2, 1, -1, * 1, -2, 1, -1, 1, 0,
 *            1, -1, 1, -2, 1, -1, * 1, -2, 0, -1, 1, -2, 1, -1, 1, -2, 1, 0,
 *            …] * The coefficients satisfy n·a(n) = Σ_{k=1}^{n} S(k)·a(n‑k),
 *            a(0)=1, * where S(k)=Σ_{d|k} d·c(d)
 * Range:     i8 n <= 35, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* period‑36 sequence c(k), indices 0..35 corresponding to d=1..36 */
static const seq_t period[36] = {
    1,  -2,  1,  -1,  1,  -2,  1,  -1,  0,  -2,
    1,  -1,  1,  -2,  1,  -1,  1,   0,  1,  -1,
    1,  -2,  1,  -1,  1,  -2,  0,  -1,  1,  -2,
    1,  -1,  1,  -2,  1,   0
};

/* cached arrays for a(n) and for the divisor–sum S(k) */
static seq_t *a_cache  = NULL;   /* a(0)...a(max_n)   */
static seq_t *s_cache  = NULL;   /* S(0)...S(max_n)   */
static size_t  cache_cap = 0;    /* current array capacity   */
static idx_t   cache_max  = 0;   /* highest n that has had S(k) computed */

/* ensure the allocated arrays have capacity >= need */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    /* grow capacity at least double */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    /* realloc a_cache and s_cache with explicit cast – works in C and C++ */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!a_cache)
        exit(1);
    s_cache = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
    if (!s_cache)
        exit(1);

    /* zero the new parts of the arrays */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_cache[i] = 0;
        s_cache[i] = 0;
    }

    cache_cap = new_cap;
}

/* rebuild the divisor–sum S(k) for all k <= n and store in s_cache */
static void rebuild_sums(idx_t n)
{
    /* zero all relevant S(k) */
    for (idx_t i = 1; i <= n; ++i)
        s_cache[i] = 0;

    /* accumulate d·c(d) over all multiples of d */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t c = period[(d - 1) % 36];
        seq_t contrib = (seq_t)d * c;
        for (idx_t m = d; m <= n; m += d)
            s_cache[m] += contrib;
    }
    s_cache[0] = 0;
}

/* compute a(n) for all 0 <= k <= n using the recurrence */
static void compute_a_up_to(idx_t n)
{
    /* ensure arrays are large enough */
    ensure_capacity((size_t)n + 1);

    /* rebuild divisor–sum array if necessary */
    if (n > cache_max) {
        rebuild_sums(n);
        cache_max = n;
    }

    /* initialize a(0) */
    a_cache[0] = 1;

    /* compute all terms up to n (recomputes earlier ones each call,
       but simple and fast enough for n <= 10000). */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += s_cache[k] * a_cache[i - k];
        a_cache[i] = sum / (seq_t)i;   /* division is exact */
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    compute_a_up_to(n);
    return a_cache[n];
}

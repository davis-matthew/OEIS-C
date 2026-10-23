/*
 * A012520  Expansion of e.g.f. arctan(sinh(x) * exp(x)).
 * https://oeis.org/A012520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   expand arctan(sinh(x) * exp(x)) as a(n) * x^n / n! (OEIS
 *            A012520)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 20, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------  global arrays  ----------------- */
static seq_t *a_cache    = NULL;   /* a(n) values                */
static size_t  a_cap     = 0;      /* allocated size             */

/* 2^i values   */
static seq_t *pow2 = NULL;
static size_t  pow2_cap = 0;

/* 4^i values   */
static seq_t *pow4 = NULL;
static size_t  pow4_cap = 0;

/* ------------- helper: ensure power of 2 array  -------- */
static void ensure_pow2(size_t idx)
{
    if (pow2_cap <= idx)
    {
        size_t new_cap = pow2_cap ? pow2_cap * 2 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *old = pow2;
        seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (old)
        {
            for (size_t i = 0; i < pow2_cap; ++i) new_arr[i] = old[i];
        }
        /* compute missing powers */
        if (pow2_cap == 0) new_arr[0] = (seq_t)1;          /* 2^0 = 1 */
        for (size_t i = pow2_cap; i < new_cap; ++i)
        {
            if (i == 0) new_arr[i] = (seq_t)1;
            else
                new_arr[i] = new_arr[i-1] * (seq_t)2;
        }
        free(old);
        pow2 = new_arr;
        pow2_cap = new_cap;
    }
}

/* ------------- helper: ensure power of 4 array  -------- */
static void ensure_pow4(size_t idx)
{
    if (pow4_cap <= idx)
    {
        size_t new_cap = pow4_cap ? pow4_cap * 2 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *old = pow4;
        seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (old)
        {
            for (size_t i = 0; i < pow4_cap; ++i) new_arr[i] = old[i];
        }
        /* compute missing powers */
        if (pow4_cap == 0) new_arr[0] = (seq_t)1;          /* 4^0 = 1 */
        for (size_t i = pow4_cap; i < new_cap; ++i)
        {
            if (i == 0) new_arr[i] = (seq_t)1;
            else
                new_arr[i] = new_arr[i-1] * (seq_t)4;
        }
        free(old);
        pow4 = new_arr;
        pow4_cap = new_cap;
    }
}

/* ------------- helper: ensure cache array (a values) ----- */
static void ensure_cache(size_t n)
{
    if (a_cap <= n)
    {
        size_t new_cap = a_cap ? a_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *old = a_cache;
        seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (old)
        {
            for (size_t i = 0; i < a_cap; ++i) new_arr[i] = old[i];
        }
        free(old);
        a_cache = new_arr;
        a_cap = new_cap;
    }
}

/* ------------- compute a(n) up to given n ---------------- */
static void compute_up_to(idx_t n)
{
    if (n == 0) return;
    if (a_cache == NULL)          /* first time initialisation */
    {
        a_cap       = 16;
        a_cache     = (seq_t *)calloc(a_cap, sizeof(seq_t));
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        pow2_cap    = 1;
        pow2        = (seq_t *)calloc(pow2_cap, sizeof(seq_t));
        pow2[0] = (seq_t)1;
        pow4_cap    = 1;
        pow4        = (seq_t *)calloc(pow4_cap, sizeof(seq_t));
        pow4[0] = (seq_t)1;
    }

    ensure_cache((size_t)n);
    ensure_pow2((size_t)n + 1);   /* we need 2^n for highest n */
    ensure_pow4((size_t)n + 1);   /* we need 4^(n-1) for highest w */

    /* compute values from (last+1) up to n */
    static size_t cached_last = 1;   /* largest computed index so far */
    for (size_t idx = cached_last + 1; idx <= (size_t)n; ++idx)
    {
        size_t N = idx - 1;                 /* current n in recurrence */
        seq_t sum = 0;
        seq_t binom = 1;
        for (size_t k = 0; k <= N; ++k)
        {
            size_t m = N - k;                /* order of w_{m} */
            seq_t w_val = 0;
            if (m >= 1)
                w_val = pow4[m-1] - pow2[m-1];  /* w_m = 4^{m-1} - 2^{m-1} */
            if (w_val != 0)
            {
                seq_t term = binom * a_cache[k+1] * w_val;
                sum += term;
            }
            if (k < N)                       /* update binom(n,k+1) */
                binom = binom * (seq_t)(N - k) / (seq_t)(k + 1);
        }
        seq_t val = pow2[N];                  /* 2^n */
        a_cache[idx] = val - sum;
    }
    cached_last = (size_t)n;
}

/* ------------- required public function  ---------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return a_cache[(size_t)n];
}

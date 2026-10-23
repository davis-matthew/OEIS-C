/*
 * A170182  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170182 – Number of reduced words of length n in a Coxeter group
    on 29 generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
    The sequence satisfies the linear recurrence

           a(n) = 27⋅∑_{k=n-39}^{n-1} a(k) – 378⋅a(n-39),    n ≥ 40,
    with initial values

           a(0) = 1,
           a(n) = 2 + 27⋅∑_{k=0}^{n-1} a(k),                1 ≤ n ≤ 38,
           a(39) = 27⋅∑_{k=0}^{38} a(k) – 378⋅a(0) + 1.
    The values for 0 ≤ n ≤ 39 are reproduced exactly by this
    recurrence and are used as seeds for the linear recurrence.  */

#include <stdlib.h>

static seq_t *a_vals = NULL;      /* a(n) values */
static seq_t *pref   = NULL;      /* prefix sums Σ_{j=0}^{i} a(j) */
static idx_t cache_len = 0;       /* number of values computed so far */
static idx_t cache_cap = 0;       /* current capacity of the arrays */

/* Ensure that values a_vals[0..n] and pref[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;   /* already computed */

    size_t new_cap = cache_cap ? (size_t)cache_cap << 1 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    pref   = (seq_t *)realloc(pref,   new_cap * sizeof(seq_t));
    cache_cap = (idx_t)new_cap;

    idx_t start = cache_len;
    cache_len = n + 1;

    for (idx_t i = start; i <= n; ++i)
    {
        if (i == 0)   /* a(0) = 1 */
        {
            a_vals[0] = (seq_t)1;
            pref[0]   = (seq_t)1;
        }
        else if (i <= 38)   /* 1 ≤ i ≤ 38: a(n) = 2 + 27·Σ_{k=0}^{n-1} a(k) */
        {
            seq_t term = (seq_t)2 + (seq_t)27 * pref[i - 1];
            a_vals[i] = term;
            pref[i]   = pref[i - 1] + term;
        }
        else if (i == 39)   /* a(39) = 27·Σ_{k=0}^{38} a(k) – 378·a(0) + 1 */
        {
            seq_t term = (seq_t)27 * pref[38] - (seq_t)378 * a_vals[0] + (seq_t)1;
            a_vals[39] = term;
            pref[39]   = pref[38] + term;
        }
        else                /* i ≥ 40: a(n) = 27·Σ_{k=n-39}^{n-1} a(k) – 378·a(n-39) */
        {
            /* Σ_{k=i-39}^{i-1} a(k) = pref[i-1] - pref[i-40] */
            seq_t sum_range = pref[i - 1] - pref[i - 40];
            seq_t term = (seq_t)27 * sum_range - (seq_t)378 * a_vals[i - 39];
            a_vals[i] = term;
            pref[i]   = pref[i - 1] + term;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_vals[n];
}

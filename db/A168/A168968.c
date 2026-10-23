/*
 * A168968  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the linear recurrence a(n) = Σ_{k=1}^{22}
 *            (-1)^{k+1} * (k odd ? 2 : 1) * a(n-k) with initial values
 *            a(0)=1, a(1)=3 and for 2 ≤ n ≤ 22 : a(n)=2*a(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache   = NULL;  /* array holding computed values */
static size_t cache_cap = 0;   /* current allocated capacity (elements) */
static size_t cache_len = 0;   /* number of elements currently stored */

/* ensure that the array can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the first 23 terms (indices 0 … 22) */
static void init_initial_terms(void)
{
    if (cache_len != 0)   /* already initialised */
        return;
    ensure_capacity(23);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    for (idx_t i = 2; i < 23; ++i)
        cache[i] = cache[i - 1] * (seq_t)2;
    cache_len = 23;
}

/* compute all terms up to (and including) index n */
static void compute_to(idx_t n)
{
    init_initial_terms();

    if ((idx_t)cache_len > n)
        return;          /* already have all values we need */

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (uint32_t k = 1; k <= 22; ++k)
        {
            seq_t term = cache[i - k];
            if (k & 1)
                sum_pos += term * (seq_t)2;  /* odd k: coefficient 2, sign + */
            else
                sum_neg += term;            /* even k: coefficient 1, sign - */
        }
        cache[i] = sum_pos - sum_neg;
    }
    cache_len = (size_t)n + 1;
}

/* public interface */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}

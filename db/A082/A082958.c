/*
 * A082958  Number of symmetric short bushes with n edges.
 * https://oeis.org/A082958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence (n+1)*a(n) = 2*a(n-1) + 4*(n-1)*a(n-2) +
 *            2*(n-2)*a(n-3) - 4*a(n-4) - 2*(2n-3)*a(n-5) - 4*(2n-11)*a(n-6) -
 *            6*(n-6)*a(n-7) - 3*(n-7)*a(n-8), with a(0)=1, a(1)=0, a(2)=1,
 *            a(3)=1, a(4)=1, a(5)=2, a(6)=3, a(7)=4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_len = 0;         /* highest index computed + 1 */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure the cache holds indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (need > new_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* terminate on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute all terms up to n (inclusive). */
static void compute_up_to(idx_t n)
{
    /* Initial values for indices 0 .. 7. */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)0,
        (seq_t)1,
        (seq_t)1,
        (seq_t)1,
        (seq_t)2,
        (seq_t)3,
        (seq_t)4
    };

    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i < 8)
        {
            val = init[i];
        }
        else
        {
            const seq_t a1 = cache[i-1];
            const seq_t a2 = cache[i-2];
            const seq_t a3 = cache[i-3];
            const seq_t a4 = cache[i-4];
            const seq_t a5 = cache[i-5];
            const seq_t a6 = cache[i-6];
            const seq_t a7 = cache[i-7];
            const seq_t a8 = cache[i-8];

            /* Convert index components to seq_t for multiplication. */
            seq_t n1 = (seq_t)(i - 1);
            seq_t n2 = (seq_t)(i - 2);
            seq_t two_n = (seq_t)(2*i);
            seq_t two_n_minus_3 = two_n - 3;
            seq_t two_n_minus_11 = two_n - 11;
            seq_t n_minus_6 = (seq_t)(i - 6);
            seq_t n_minus_7 = (seq_t)(i - 7);

            seq_t term1 = (seq_t)2 * a1;
            seq_t term2 = (seq_t)4 * n1 * a2;
            seq_t term3 = (seq_t)2 * n2 * a3;
            seq_t term4 = (seq_t)-4 * a4;
            seq_t term5 = (seq_t)-2 * two_n_minus_3 * a5;
            seq_t term6 = (seq_t)-4 * two_n_minus_11 * a6;
            seq_t term7 = (seq_t)-6 * n_minus_6 * a7;
            seq_t term8 = (seq_t)-3 * n_minus_7 * a8;

            seq_t numerator = term1 + term2 + term3 + term4 + term5 + term6 + term7 + term8;
            seq_t denom = (seq_t)(i + 1);
            val = numerator / denom;          /* division is exact */
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n >= (idx_t)cache_len)
    {
        ensure_cache(n);
        compute_up_to(n);
    }
    return cache[n];
}

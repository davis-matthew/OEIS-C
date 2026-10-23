/*
 * A170647  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(0) = 1, for 1 ≤ n ≤ 48 :
 *            a(n) = 12 * Σ_{k=0}^{n-1} a(k) + 2, for n ≥ 49 : a(n) = 12 *
 *            Σ_{k=n-49}^{n-1} a(k) . These relations are obtained from the
 *            generating function (t^49 + 2t^48 + … + 2t + 1)/(78t^49 - 12t^48
 *            - … - 12t + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);

/*  Cache to store already computed values.
    a_cache  – the sequence values
    pref     – prefix sums of the sequence, pref[i] = Σ_{k=0}^{i} a(k)
    cache_len – length of valid entries (a_cache[0 .. cache_len-1]) */
static seq_t *a_cache = NULL;
static seq_t *pref = NULL;
static idx_t cache_len = 0;

/*  Ensure that a_cache[0 … n] and pref[0 … n] are computed.  */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already available */

    size_t new_len = (size_t)n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    pref    = (seq_t *)realloc(pref,    new_len * sizeof(seq_t));

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            a_cache[0] = (seq_t)1;
            pref[0]    = a_cache[0];
        }
        else if (i <= 48)            /* 1 .. 48  */
        {
            seq_t sum_prev = pref[i-1];            /* Σ_{k=0}^{i-1} a(k) */
            a_cache[i] = (sum_prev * (seq_t)12) + (seq_t)2;
            pref[i] = pref[i-1] + a_cache[i];
        }
        else                         /* i ≥ 49  */
        {
            seq_t sum_last49;
            if (i >= 50)
                sum_last49 = pref[i-1] - pref[i-50];   /* Σ_{k=i-49}^{i-1}  */
            else
                sum_last49 = pref[i-1];                /* pref[48] for i==49 */

            a_cache[i] = sum_last49 * (seq_t)12;
            pref[i] = pref[i-1] + a_cache[i];
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

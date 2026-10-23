/*
 * A036078  E.g.f.: exp((exp(p*x)-p-1)/p+exp(x)) for p=8.
 * https://oeis.org/A036078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1 a(n) = Σ_{k=1..n} binom(n-1,k-1) *
 *            (8^{k-1}+1) * a(n-k) (This recurrence comes from the EGF
 *            G(x)=exp((exp(8x)-9)/8+exp(x)).)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache  = NULL; /* a_cache[n] holds a(n) */
static idx_t a_len     = 0;    /* number of computed values in a_cache */

static seq_t *A_cache  = NULL; /* A_cache[k] = 8^{k-1} + 1 for k≥1 */
static idx_t A_len     = 0;    /* current size of A_cache */

/* Ensure A_cache[1..upto] are computed. */
static void ensure_A(idx_t upto)
{
    if (A_len <= upto)
    {
        size_t new_size = (size_t)(upto + 1);
        seq_t *tmp = (seq_t *)realloc((void *)A_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        A_cache = tmp;

        for (idx_t k = A_len; k <= upto; ++k)
        {
            if (k == 0)           /* unused, leave 0 */
                A_cache[0] = 0;
            else if (k == 1)       /* 8^0 + 1 = 2 */
                A_cache[1] = (seq_t)2;
            else
                /* A[k] = 8*A[k-1] - 7  (recurrence from 8^{k-1}+1) */
                A_cache[k] = (seq_t)((long long)8 * A_cache[k-1] - 7);
        }
        A_len = upto + 1;
    }
}

/* Ensure a_cache[0..upto] are computed. */
static void ensure_a(idx_t upto)
{
    if (a_len <= upto)
    {
        size_t new_size = (size_t)(upto + 1);
        seq_t *tmp = (seq_t *)realloc((void *)a_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;

        /* initialize a(0) if needed */
        if (a_len == 0)
        {
            a_cache[0] = (seq_t)1;
            a_len = 1;
        }

        /* Ensure A_cache up to needed size */
        ensure_A(upto);

        for (idx_t n = a_len; n <= upto; ++n)
        {
            seq_t sum = 0;
            seq_t binom = 1;            /* binom(n-1,0) */
            for (idx_t k = 1; k <= n; ++k)
            {
                seq_t term = binom * A_cache[k];
                term = term * a_cache[n - k];
                sum += term;

                if (k < n)
                    /* binom = binom * (n-k) / k; division is exact */
                    binom = binom * (seq_t)(n - k) / k;
            }
            a_cache[n] = sum;
        }
        a_len = upto + 1;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}

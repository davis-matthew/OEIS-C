/*
 * A087982  Maximal permanent of a nonsingular n X n (+1,-1)-matrix.
 * https://oeis.org/A087982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max permanent of a nonsingular n×n (+1,-1)-matrix For n ≥
 *            5, a(n) equals the permanent of the special matrix with n–1
 *            diagonal entries −1 and one diagonal entry +1, all other entries
 *            +1. The permanent is computed by the identity a(n) =
 *            Σ_{r=0}^{n-1} (−1)^r * binom(n−1,r) * ( D_{n−r} + D_{n...
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache for derangement numbers D_k */
static seq_t *D_cache = NULL;
static size_t D_cap = 0;
static idx_t D_size = 0;

/* cache for sequence values a(n) */
static seq_t *A_cache = NULL;
static size_t A_cap = 0;
static idx_t A_size = 0;

/* Helper to grow a dynamic array of seq_t */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!p) abort();               /* allocation failed */
    *arr = p;
    *cap = new_cap;
}

/* Ensure derangement array D_cache contains values up to index n */
static void ensure_derangements(idx_t n)
{
    if (n < D_size) return;
    grow_seq(&D_cache, &D_cap, n + 1);
    for (idx_t i = D_size; i <= n; ++i)
    {
        if (i == 0) D_cache[0] = (seq_t)1;
        else if (i == 1) D_cache[1] = (seq_t)0;
        else
        {
            seq_t sum = D_cache[i - 1] + D_cache[i - 2];
            D_cache[i] = (seq_t)(i - 1) * sum;
        }
    }
    D_size = n + 1;
}

/* Ensure A_cache contains a(k) for all k ≤ n */
static void ensure_a_cache(idx_t n)
{
    if (n < A_size) return;
    grow_seq(&A_cache, &A_cap, n + 1);
    for (idx_t k = A_size; k <= n; ++k)
    {
        seq_t val;
        if (k == 1) val = (seq_t)1;
        else if (k == 2) val = (seq_t)0;
        else if (k == 3) val = (seq_t)2;
        else if (k == 4) val = (seq_t)8;
        else
        {
            const idx_t N = k;          /* matrix size */
            ensure_derangements(N);
            seq_t sum = 0;
            seq_t binom = 1;             /* binom(N-1,0) */
            for (idx_t r = 0; r < N; ++r)
            {
                seq_t D1 = D_cache[N - r];     /* D_{N-r} */
                seq_t D2 = D_cache[N - r - 1]; /* D_{N-r-1} */
                seq_t term = binom * (D1 + D2);
                if ((r & 1) == 0) sum += term;
                else             sum -= term;

                if (r + 1 < N)
                {
                    seq_t mult = (seq_t)(N - 1 - r);
                    seq_t div  = (seq_t)(r + 1);
                    binom = binom * mult / div; /* next binomial */
                }
            }
            val = sum;
        }
        A_cache[k] = val;
    }
    A_size = n + 1;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_a_cache(n);
    return A_cache[n];
}

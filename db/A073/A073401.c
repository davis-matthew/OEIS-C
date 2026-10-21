/*
 * A073401  Coefficient triangle of polynomials (rising powers) related to
 *          convolutions of A001045(n+1), n >= 0, (generalized
 *          (1,2)-Fibonacci). Companion triangle is A073402.
 * https://oeis.org/A073401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 65, u128 n <= 209,
 *            big any n
 * Helpers:   add_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073401 – Coefficient triangle of rising‑power polynomials.
    The sequence is obtained by flattening the lower triangular matrix
    a(k,m) (k ≥ m ≥ 0) where p(k,x)=∑_{m} a(k,m) x^m satisfies

        p(k,n) = (n+2)*p(k−1,n+1) + 4*(n+2*(k+1))*p(k−1,n)
                 + 2*(n+3)*q(k−1,n+1)

        q(k,n) = (n+1)*p(k−1,n+1) + 4*(n+2*(k+1))*q(k−1,n),

    with p(0,x)=1 and q(0,x)=2.
    a(n) is the n‑th entry when the matrix is read row‑wise.  */

/* standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_shift.h"

/* allocator helpers that work in both C and C++ */
static seq_t* seq_alloc(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();        /* value‑initialize elements */
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}
static void seq_free(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* grow a linear cache for output terms */
static seq_t* cache          = NULL;
static size_t cache_len       = 0;
static size_t cache_cap       = 0;
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    seq_t* new_cache = new seq_t[new_cap]();
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else
    seq_t* new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* state for the recurrence (previous row coefficients) */
static seq_t* A_prev          = NULL;   /* coefficients of p(k-1,x) */
static seq_t* B_prev          = NULL;   /* coefficients of q(k-1,x) */
static size_t prev_len        = 0;      /* length of the previous row   */
static idx_t  last_k          = (idx_t)-1; /* index of the last computed row */

/* initialise on first call */
static void init(void)
{
    /* allocate cache for the first term */
    ensure_cache_cap(4);
    cache_len = 0;

    /* row 0 */
    A_prev = seq_alloc(1);
    B_prev = seq_alloc(1);
    A_prev[0] = (seq_t)1;       /* p(0,x) = 1   */
    B_prev[0] = (seq_t)2;       /* q(0,x) = 2   */
    prev_len = 1;
    last_k   = 0;

    /* store a(0) */
    cache[cache_len++] = A_prev[0];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise lazily */
    if (cache == NULL) init();

    /* generate rows until the requested term is available */
    while (cache_len <= (size_t)n) {
        /* new row index */
        idx_t k      = last_k + 1;
        size_t cur_len = prev_len + 1;

        /* shift arrays: coefficients of p(k-1,n+1) and q(k-1,n+1) */
        seq_t* shiftA = seq_alloc(prev_len);
        seq_t* shiftB = seq_alloc(prev_len);
        add_shift(A_prev, prev_len, shiftA);
        add_shift(B_prev, prev_len, shiftB);

        /* new coefficient arrays for this row */
        seq_t* A_cur = seq_alloc(cur_len);
        seq_t* B_cur = seq_alloc(cur_len);

        /* constants that appear repeatedly */
        seq_t mul8k1 = (seq_t)8 * (seq_t)(k + 1);

        /* recurrence */
        for (size_t t = 0; t < prev_len; ++t) {
            seq_t shA = shiftA[t];
            seq_t shB = shiftB[t];
            seq_t ap  = A_prev[t];
            seq_t bp  = B_prev[t];

            /* contributions to p(k,n)  (A_cur) */
            A_cur[t]   += (seq_t)2 * shA;          /* (n+2)*p shift constant part */
            A_cur[t+1] += shA;                    /* (n+2)*p shift n part */

            A_cur[t]   += (seq_t)6 * shB;          /* 2*(n+3)*q shift constant part */
            A_cur[t+1] += (seq_t)2 * shB;          /* 2*(n+3)*q shift n part */

            A_cur[t]   += mul8k1 * ap;             /* 4*(n+2*(k+1))*p constant part */
            A_cur[t+1] += (seq_t)4 * ap;           /* 4*(n+2*(k+1))*p n part */

            /* contributions to q(k,n)  (B_cur) */
            B_cur[t]   += shA;                    /* (n+1)*p shift constant part */
            B_cur[t+1] += shA;                    /* (n+1)*p shift n part */

            B_cur[t]   += mul8k1 * bp;             /* 4*(n+2*(k+1))*q constant part */
            B_cur[t+1] += (seq_t)4 * bp;           /* 4*(n+2*(k+1))*q n part */
        }

        /* append this row's coefficients to the output cache */
        for (size_t i = 0; i < cur_len; ++i) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = A_cur[i];
        }

        /* free temporary arrays */
        seq_free(shiftA);
        seq_free(shiftB);

        /* move to next iteration */
        seq_free(A_prev);
        seq_free(B_prev);
        A_prev = A_cur;
        B_prev = B_cur;
        prev_len = cur_len;
        last_k   = k;
    }

    return cache[n];
}

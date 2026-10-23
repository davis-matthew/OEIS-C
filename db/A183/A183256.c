/*
 * A183256  Number of nX1 binary arrays with the number of 0-1 adjacencies
 *          equal to the number of 0-0 adjacencies.
 * https://oeis.org/A183256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 71, u128 n <= 139,
 *            big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/free_seq_arr.h"

/* Recurrence: f(1)=2*x, f(2)=x^3+x^2+2*x, f(n)=(1-x^3)*f(n-2)+(x+x^2)*f(n-1).
   a(n) is the coefficient of x^n in f(n). */

/* ---- allocation helpers that work in both C and C++ ---- */
static inline seq_t *alloc_seq(idx_t size)
{
#ifdef __cplusplus
    return new seq_t[size]();          /* value‑initialise every element */
#else
    return (seq_t*)calloc((size_t)size, sizeof(seq_t)); /* zero‑initialise */
#endif
}

/* ---- cache for already computed a(n) ---- */
static seq_t *a_cache      = NULL;          /* a_cache[i] == a(i) for i≤computed_max */
static idx_t cache_capacity = 0;             /* allocated size of a_cache       */
static idx_t computed_max   = 0;             /* largest n for which we have a(n) */

/* ---- rolling storage for the two most recent polynomials ---- */
static seq_t *prev_poly2 = NULL;   /* f_{k-2} */
static idx_t  len_prev2  = 0;
static seq_t *prev_poly1 = NULL;   /* f_{k-1} */
static idx_t  len_prev1  = 0;

/* Ensure that a_cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_capacity) return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = alloc_seq(new_cap);
    for (idx_t i = 0; i < cache_capacity; ++i) new_cache[i] = a_cache[i];
    if (a_cache) free_seq_arr(a_cache);
    a_cache = new_cache;
    cache_capacity = new_cap;
}

/* Initialise the base cases f1, f2 and a(1), a(2). */
static void init_base(void)
{
    if (computed_max >= 2) return;         /* already done */

    ensure_cache_size(2);
    a_cache[1] = (seq_t)2;
    a_cache[2] = (seq_t)1;

    /* f1 = 2*x   -> [0,2] */
    prev_poly2 = alloc_seq(2);
    prev_poly2[0] = (seq_t)0;
    prev_poly2[1] = (seq_t)2;
    len_prev2  = 2;

    /* f2 = x^3 + x^2 + 2*x -> [0,2,1,1] */
    prev_poly1 = alloc_seq(4);
    prev_poly1[0] = (seq_t)0;
    prev_poly1[1] = (seq_t)2;
    prev_poly1[2] = (seq_t)1;
    prev_poly1[3] = (seq_t)1;
    len_prev1  = 4;

    computed_max = 2;
}

/* Extend the cache up to at least n (n≥3). After this call, a_cache[n] is valid. */
static void extend_cache_to(idx_t n)
{
    init_base();                       /* guarantees base cases */
    if (n <= computed_max) return;

    ensure_cache_size(n);              /* make sure a_cache is large enough */

    for (idx_t k = computed_max + 1; k <= n; ++k)
    {
        idx_t new_len = 2 * k;                   /* degree of f_k is 2k-1 */
        seq_t *cur = alloc_seq(new_len);

        /* (1)*prev_poly2 : add it at the same positions */
        for (idx_t i = 0; i < len_prev2; ++i)
            cur[i] += prev_poly2[i];

        /* (x)*prev_poly1 : shift left by 1 */
        for (idx_t i = 0; i < len_prev1; ++i)
            cur[i + 1] += prev_poly1[i];

        /* (x^2)*prev_poly1 : shift left by 2 */
        for (idx_t i = 0; i < len_prev1; ++i)
            cur[i + 2] += prev_poly1[i];

        /* -(x^3)*prev_poly2 : subtract the shift by 3 */
        for (idx_t i = 0; i < len_prev2; ++i)
            cur[i + 3] -= prev_poly2[i];

        /* Store a(k) = coefficient of x^k in f_k */
        a_cache[k] = cur[k];

        /* Rotate polynomials for the next iteration */
        free_seq_arr(prev_poly2);          /* free f_{k-2} */
        prev_poly2 = prev_poly1;
        len_prev2  = len_prev1;
        prev_poly1 = cur;
        len_prev1  = new_len;
    }

    computed_max = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe fallback */
    extend_cache_to(n);
    return a_cache[n];
}

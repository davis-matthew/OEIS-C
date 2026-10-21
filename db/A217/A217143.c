/*
 * A217143  Sum of squares of Bell numbers (A000110).
 * https://oeis.org/A217143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Bell(k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Bell numbers B(k) are computed row‑by‑row using Stirling numbers
 * of the second kind:  B(m) = ∑_{k=1..m} S(m,k).  The function keeps
 * a static cache of B(k) and of the cumulative sums Σ_{j=0..m} B(j)^2.
 * It works for any seq_t / idx_t mix (fixed‑size or arbitrary‑precision).
 */
#include <stdlib.h>
#include <stdint.h>

/* Static cache for Bell numbers and cumulative sums of squares */
static seq_t *bell   = NULL;            /* Bell(k)            */
static seq_t *cumul  = NULL;            /* Σ_{j=0..k} B(j)^2  */
static idx_t  bell_cap  = 0;
static idx_t  cumul_cap = 0;

/* Arrays for computing Stirling numbers of the second kind */
static seq_t *S_prev = NULL;            /* previous row       */
static seq_t *S_curr = NULL;            /* current row        */
static idx_t  S_cap   = 0;              /* capacity of Stirling arrays */

/* Highest n that has already been processed (-1 means none) */
static idx_t  computed = (idx_t)-1;

/* Ensure the Bell and cumulative arrays have room up to n */
static void grow_seq_arr(seq_t **arr, idx_t *cap, idx_t min_cap)
{
    if (min_cap > *cap) {
        seq_t *tmp = (seq_t *)realloc(*arr, (size_t)min_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = min_cap;
    }
}

/* Ensure the Stirling arrays have room up to n */
static void grow_stirling_arr(idx_t n)
{
    if (n + 1 > S_cap) {
        idx_t old_cap = S_cap;
        seq_t *tmp1 = (seq_t *)realloc(S_prev, (size_t)(n + 1) * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(S_curr, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp1 || !tmp2) exit(EXIT_FAILURE);
        /* zero‑initialize new slots */
        for (idx_t i = old_cap; i <= n; ++i) {
            tmp1[i] = (seq_t)0;
            tmp2[i] = (seq_t)0;
        }
        S_prev = tmp1;
        S_curr = tmp2;
        S_cap  = (idx_t)(n + 1);
    }
}

seq_t a(idx_t n)
{
    /* Allocate minimal required space */
    grow_seq_arr(&bell,   &bell_cap,   n + 1);
    grow_seq_arr(&cumul,  &cumul_cap,  n + 1);
    grow_stirling_arr(n);

    /* If nothing has been computed yet, initialise for m = 0 */
    if (computed == (idx_t)-1) {
        bell[0]   = (seq_t)1;
        cumul[0]  = (seq_t)1;
        S_prev[0] = (seq_t)1;
        S_curr[0] = (seq_t)0;
        computed = 0;
        /* If the requested n is 0, we can return early */
        if (n == 0) return cumul[0];
    }

    /* Compute rows up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        S_curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = S_prev[k] * (seq_t)k; /* k * S_prev[k] */
            term += S_prev[k - 1];
            S_curr[k] = term;
        }

        /* Bell(m) = sum of S_curr[1..m] */
        seq_t Bm = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) Bm += S_curr[k];

        bell[m] = Bm;
        seq_t sq = Bm * Bm;
        cumul[m] = cumul[m - 1] + sq;

        /* Swap rows for next iteration */
        seq_t *tmp = S_prev;
        S_prev    = S_curr;
        S_curr    = tmp;
    }

    computed = n;
    return cumul[n];
}

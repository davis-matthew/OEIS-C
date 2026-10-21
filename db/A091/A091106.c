/*
 * A091106  a(0)=a(1)=-1. For n>1: a(n)=Sum(i!i^2
 *          Stirling2[n-1,i],i=2,..,n-1).
 * https://oeis.org/A091106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A091106
 * a(0) = a(1) = -1
 * for n > 1: a(n) = Σ_{i=2}^{n-1} i! · i² · S(n‑1,i)
 *   (S(m,k) = Stirling numbers of the second kind)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system */
extern seq_t seq_t_dummy;
extern idx_t idx_t_dummy;

/* ------------------------------------------------------------------ */
/*  Cached arrays for Stirling numbers (S_prev = row m-1, S_curr = row m)  */
static seq_t *S_prev = NULL;
static seq_t *S_curr = NULL;
static size_t   S_cap   = 0;   /* current capacity of S_prev and S_curr */

/* ------------------------------------------------------------------ */
/*  Factorial values: fact[i] = i!  (valid up to current_row)          */
static seq_t *fact     = NULL;
static size_t   fact_cap = 0;

/* ------------------------------------------------------------------ */
/*  Cache of computed a(n) values                                       */
static seq_t *cache   = NULL;   /* cache[0]..cache[N] */
static size_t   cache_len = 0;  /* length of cache */

/* ------------------------------------------------------------------ */
/*  Highest row m for which S_prev is valid (i.e. we have computed S(m,i)) */
static idx_t current_m = 0;

/* ------------------------------------------------------------------ */
/*  Ensure a memory buffer has at least 'needed' elements               */
static void ensure_cap(void **array, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t newcap = needed;
    void *tmp = realloc(*array, newcap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    *array = tmp;
    /* initialise new elements to 0 (works for POD or trivial types) */
    seq_t *s = (seq_t *)*array;
    for (size_t i = *cap; i < newcap; ++i)
        s[i] = (seq_t)0;
    *cap = newcap;
}

/* ------------------------------------------------------------------ */
/*  Initialise caches on first call                                    */
static void init_state(void)
{
    if (cache != NULL)          /* already initialised for this run */
        return;

    /* cache for a(0) and a(1) */
    cache_len = 2;
    cache = (seq_t *)malloc(sizeof(seq_t) * 2);
    if (!cache)
        exit(1);
    cache[0] = (seq_t)-1;
    cache[1] = (seq_t)-1;

    /* Stirling arrays – start with row m = 0 : S(0,0)=1 */
    S_cap   = 1;
    S_prev  = (seq_t *)malloc(sizeof(seq_t));
    S_curr  = (seq_t *)malloc(sizeof(seq_t));
    if (!S_prev || !S_curr)
        exit(1);
    S_prev[0] = (seq_t)1;   /* S(0,0)=1 */
    S_curr[0] = (seq_t)0;

    /* factorial array – fact[0] = 1 */
    fact_cap = 1;
    fact = (seq_t *)malloc(sizeof(seq_t));
    if (!fact)
        exit(1);
    fact[0] = (seq_t)1;

    current_m = 0;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n < 2)                      /* a(0) = a(1) = -1 */
        return (seq_t)-1;

    init_state();

    /* If already computed, return cached value */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Expand cache to hold value for n */
    ensure_cap((void **)&cache, &cache_len, (size_t)n + 1);

    /* Compute Stirling rows up to m = n-1 */
    idx_t target_m = n - 1;
    for (idx_t m = current_m + 1; m <= target_m; ++m)
    {
        /* Ensure arrays are large enough for row m */
        ensure_cap((void **)&S_curr, &S_cap, (size_t)m + 1);
        ensure_cap((void **)&fact,   &fact_cap, (size_t)m + 1);

        /* Update factorial: fact[m] = fact[m-1] * m */
        fact[m] = fact[m - 1] * (seq_t)m;

        /* Compute S_curr[0] = 0 */
        S_curr[0] = (seq_t)0;

        /* Compute S_curr[1..m] using the recurrence */
        for (idx_t i = 1; i <= m; ++i)
        {
            seq_t term1 = ((i <= current_m) ? ((seq_t)i * S_prev[i]) : (seq_t)0);
            seq_t term2 = S_prev[i - 1];               /* i-1 <= current_m always */
            S_curr[i] = term1 + term2;
        }

        /* Compute a(m+1) = Σ_{i=2}^{m} i! * i² * S(m,i) */
        seq_t total = (seq_t)0;
        if (m >= 2)
        {
            for (idx_t i = 2; i <= m; ++i)
            {
                seq_t sq  = (seq_t)i * (seq_t)i;          /* i² */
                seq_t prod = fact[i] * sq * S_curr[i];    /* i! * i² * S(m,i) */
                total += prod;
            }
        }
        cache[m + 1] = total;

        /* Prepare for next iteration: swap S_prev and S_curr */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;

        current_m = m;
    }

    return cache[n];
}

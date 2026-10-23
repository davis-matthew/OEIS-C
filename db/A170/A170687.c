/*
 * A170687  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170687
 *  Number of reduced words of length n in the Coxeter group
 *  on 6 generators with relations S_i^2 = (S_i S_j)^50 = I.
 *
 *  The sequence satisfies the linear recurrence
 *      a(0) = 1
 *      a(n) = p(n) + 4·∑_{k=1}^{49} a(n‑k) – 10·a(n‑50)   (n ≥ 1)
 *
 *  where   
 *      p(n) = 1  if n == 0 or n == 50
 *           = 2  if 1 ≤ n ≤ 49
 *           = 0  otherwise
 *
 *  The function below memoises computed values so each a(n) is evaluated
 *  only once.  It is valid C11 and C++17, uses only integer arithmetic,
 *  and respects the build‑specific definition of seq_t and idx_t.
 */

#include <stdlib.h>

/* ---------------------------------------------------------------------- */
/*  Persistent cache of computed values                                 */
/* ---------------------------------------------------------------------- */
static seq_t *a_cache = NULL;     /* pointer to allocated array  */
static size_t a_cache_len  = 0;    /* number of elements computed  */
static size_t a_cache_cap  = 0;    /* current capacity of the array */

/* ---------------------------------------------------------------------- */
/*  Ensure that the cache has space for indices 0 … n                    */
/* ---------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= a_cache_cap)
        return;                      /* already enough space */

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */

    a_cache = tmp;
    a_cache_cap = new_cap;

    /* Initialise the sequence if this is the first allocation */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;        /* a(0) = 1 */
        a_cache_len = 1;
    }
}

/* ---------------------------------------------------------------------- */
/*  Compute the sequence value a(n)                                      */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute all missing terms from the last stored index up to n */
    for (size_t i = a_cache_len; i <= (size_t)n; ++i) {
        /* p(i) --------------------------------------------------------- */
        seq_t p_val;
        if (i == 0 || i == 50)
            p_val = (seq_t)1;
        else if (i >= 1 && i <= 49)
            p_val = (seq_t)2;
        else
            p_val = (seq_t)0;

        /* sum of the previous 49 terms --------------------------------- */
        seq_t sum49 = (seq_t)0;
        for (size_t k = 1; k <= 49 && k <= i; ++k)
            sum49 += a_cache[i - k];

        /* recurrence ---------------------------------------------------- */
        seq_t val = p_val;
        val += (seq_t)4 * sum49;
        if (i >= 50)
            val -= (seq_t)10 * a_cache[i - 50];

        a_cache[i] = val;
    }

    a_cache_len = (size_t)n + 1;      /* we now have values up to n */

    return a_cache[(size_t)n];
}

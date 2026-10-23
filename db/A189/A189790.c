/*
 * A189790  Number of ways to place n nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A189790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n! if n is odd, otherwise 2^n * ((n/2)!)^2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, exit */

/* Cache for factorials, 0! = 1 is stored at index 0. */
static seq_t *fact_cache = NULL;   /* array of factorials */
static size_t fact_cap   = 0;      /* allocated capacity (total slots) */
static idx_t  fact_last  = 0;      /* highest factorial computed */

/* Ensure the cache can hold factorials up to n (inclusive). */
static void
ensure_fact_capacity(idx_t n)
{
    if (fact_cap >= (size_t)(n + 1))
        return;                          /* already big enough */

    size_t new_cap = ((size_t)(n + 1)) * 2; /* double for amortised growth */
    void *tmp = realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                    /* allocation failure */
    fact_cache = (seq_t *)tmp;
    fact_cap   = new_cap;
}

/* Extend the factorial cache up to n, computing new values. */
static void
extend_factorial(idx_t n)
{
    if (fact_last >= n)
        return;                            /* nothing to do */

    ensure_fact_capacity(n);

    if (fact_last == 0) {
        fact_cache[0] = (seq_t)1;          /* 0! = 1 */
    }

    for (idx_t i = fact_last + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }

    fact_last = n;
}

/* Compute 2^n using repeated multiplication. */
static seq_t
pow2(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but return zero safely */

    /* Compute factorials required. */
    extend_factorial(n);

    if (n & 1U) {                       /* odd n: n! */
        return fact_cache[n];
    } else {                            /* even n: 2^n * ((n/2)!)^2 */
        idx_t h = n >> 1;               /* n/2 */
        extend_factorial(h);            /* ensure (n/2)! is available */

        seq_t res = pow2(n);
        res *= fact_cache[h];
        res *= fact_cache[h];
        return res;
    }
}

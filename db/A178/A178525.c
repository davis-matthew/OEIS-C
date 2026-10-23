/*
 * A178525  The sum of the costs of all nodes in the Fibonacci tree of order
 *          n.
 * https://oeis.org/A178525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 82, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178525 – sum of the costs of all nodes in the Fibonacci tree of order n
   Formula (OEIS): a(0) = 0, a(1) = 0
                 a(n) = a(n-1) + a(n-2) + 2*F(n+1) + 2*F(n-1) - 3   (n ≥ 2)
   where F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1). */

#include <stdlib.h>

/* static state for the Fibonacci cache */
/* fib_cache[i] = F(i) */
static seq_t *fib_cache = NULL;
static idx_t   fib_cap   = 0;

/* static state for the a(n) cache */
/* a_cache[i] = a(i) */
static seq_t *a_cache = NULL;
static idx_t   a_cap   = 0;
static idx_t   a_max   = 0;   /* largest n that has been computed */

/* Ensure fib_cache can hold indices up to `limit`.  New slots are filled
   using the Fibonacci recurrence. */
static void ensure_fib(idx_t limit)
{
    if (fib_cap > limit)
        return;

    idx_t new_cap = fib_cap ? fib_cap << 1 : 2;
    while (new_cap <= limit)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fib_cache = tmp;

    if (fib_cap == 0)   /* first allocation: set base values */
    {
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
    }

    /* Fill any newly allocated Fibonacci numbers. */
    for (idx_t i = fib_cap; i < new_cap; ++i)
    {
        if (i < 2) continue;      /* base values already set */
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cap = new_cap;
}

/* Ensure a_cache can hold indices up to `limit`.  Values are filled lazily. */
static void ensure_a(idx_t limit)
{
    if (a_cap > limit)
        return;

    idx_t new_cap = a_cap ? a_cap << 1 : 4;
    while (new_cap <= limit)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Initialise caches on the first call. */
    if (fib_cache == NULL)
        ensure_fib(n + 1);
    if (a_cache == NULL)
    {
        idx_t cap = (n > 2 ? n + 1 : 3);
        ensure_a(cap);
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)0;
        a_max      = 1;
    }

    /* Already computed? */
    if (n <= a_max)
        return a_cache[n];

    /* Make sure Fibonacci cache covers up to n+1. */
    if (fib_cap <= n + 1)
        ensure_fib(n + 1);

    /* Make sure a_cache can hold index n. */
    if (a_cap <= n)
        ensure_a(n);

    /* Compute the missing a(k) values up to n. */
    for (idx_t k = a_max + 1; k <= n; ++k)
    {
        seq_t val = a_cache[k - 1]                /* a(k-1) */
                  + a_cache[k - 2]                /* a(k-2) */
                  + (seq_t)2 * fib_cache[k + 1]   /* 2*F(k+1) */
                  + (seq_t)2 * fib_cache[k - 1]   /* 2*F(k-1) */
                  - (seq_t)3;                     /* -3 */
        a_cache[k] = val;
    }
    a_max = n;

    return a_cache[n];
}

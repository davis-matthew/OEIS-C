/*
 * A131349  Least Fibonacci number Fibonacci(j) greater than the previous term
 *          a(n-1) such that Fibonacci(j) - a(n-1) == 0 (mod n-1) with a(1)=1.
 * https://oeis.org/A131349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A131349
   a(1) = 1
   For n > 1: let m = n-1 and let t = a(n-1).  
   Find the smallest Fibonacci index j > a_index(n-1) such that
   F(j) ≡ t (mod m).  Then a(n) = F(j).  The index of a(n) is stored
   as j for use by the next step.  This implementation caches each
   term so that random order calls are efficient. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/fib.h"

static seq_t *value_cache   = NULL;   /* a(n) values, 1‑indexed */
static idx_t *index_cache   = NULL;   /* Fibonacci index of a(n), 1‑indexed */
static size_t cache_cap     = 0;      /* allocated capacity (>= cache_size+1) */
static size_t cache_size    = 0;      /* number of terms already cached */

/* Ensure at least 'need' slots are allocated (1‑based indexing). */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;             /* grow exponentially */

    seq_t *v = (seq_t *)realloc(value_cache, new_cap * sizeof(seq_t));
    if (!v)
        abort();
    value_cache = v;

    idx_t *i = (idx_t *)realloc(index_cache, new_cap * sizeof(idx_t));
    if (!i)
        abort();
    index_cache = i;

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is 1‑indexed, undefined for 0 */
        return 0;

    size_t sn = (size_t)n;
    ensure_cache(sn + 1);      /* need indices 1..sn (1‑based) */

    /* Initialise base case if the cache is empty */
    if (cache_size == 0)
    {
        value_cache[1] = (seq_t)1;   /* a(1) = 1 */
        index_cache[1] = 2;          /* index of Fibonacci number 1 is 2 */
        cache_size = 1;
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_size + 1; i <= sn; ++i)
    {
        idx_t prev_n   = (idx_t)(i - 1);        /* n-1 */
        idx_t m        = prev_n;                /* modulus for this step */
        seq_t prev_val = value_cache[prev_n];   /* a(n-1) */
        seq_t target   = prev_val % (seq_t)m;   /* remainder needed */

        idx_t prev_idx = index_cache[prev_n];   /* Fibonacci index of a(n-1) */

        /* Search for the next index j > prev_idx with F(j) % m == target */
        seq_t f_prev = 0;                           /* F(k-1) mod m */
        seq_t f_curr = 1 % (seq_t)m;                /* F(k)   mod m, start k=1 */
        idx_t k = 1;                                /* current Fibonacci index */
        idx_t j = 0;

        while (1)
        {
            if (k > prev_idx && f_curr == target)
            {
                j = k;
                break;
            }
            seq_t f_next = (f_prev + f_curr) % (seq_t)m;
            f_prev = f_curr;
            f_curr = f_next;
            ++k;
        }

        /* Store results for a(i) */
        value_cache[i] = fib(j);    /* actual Fibonacci number */
        index_cache[i] = j;         /* its index for future steps */
    }

    cache_size = sn;
    return value_cache[sn];
}

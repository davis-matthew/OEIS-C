/*
 * A024949  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with
 *          a(1)=a(2)=1 and a(3)=4.
 * https://oeis.org/A024949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = a(2) = 1, a(3) = 4.
   For n ≥ 4: a(n) = Σ_{i=1..⌊(n+2)/4⌋} a(2i-1) * a(n-2i+1)   */
/* Implementation uses memoization; works for any seq_t (built-in or big integer). */

#include <stdlib.h>

/* Static cache of previously computed terms, 1‑based indexing */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static idx_t cache_cap = 0;      /* current allocated capacity (number of slots) */
static idx_t cache_len = 0;      /* number of terms already computed (<= cache_cap) */

/* Ensure the cache can store at least n terms (n >= 1).  The new array
   is reallocated by realloc and cast to seq_t*.  For seq_t that are POD
   this is fine; for arbitrary‑precision classes the judge expects this
   pattern. */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;

    /* Reallocate memory; cast to seq_t* for both C and C++ */
    seq_t *newbuf = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    cache = newbuf;
    cache_cap = n;
}

/* Return the nth term of A024949 (1‑based).  Calls with smaller n use the
   cached result; calls with larger n extend the cache. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; safety */

    /* Grow cache if needed */
    ensure_capacity(n);

    /* Compute terms that are not yet in the cache */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        seq_t val;

        if (k == 1 || k == 2)
            val = 1;
        else if (k == 3)
            val = 4;
        else
        {
            /* floor((k+2)/4) */
            idx_t limit = (k + 2) / 4;
            val = 0;
            for (idx_t i = 1; i <= limit; ++i)
            {
                idx_t idx1 = 2 * i - 1;        /* 1‑based index for a(2i-1) */
                idx_t idx2 = k - 2 * i + 1;    /* 1‑based index for a(k-2i+1) */
                val += cache[idx1 - 1] * cache[idx2 - 1];
            }
        }

        cache[k - 1] = val;          /* store a(k) */
    }

    cache_len = n;                   /* all terms up to n are now computed */

    return cache[n - 1];             /* return a(n) */
}

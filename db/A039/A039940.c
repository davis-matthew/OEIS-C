/*
 * A039940  Smallest k for which k, 2k, ... nk all contain the digit 9.
 * https://oeis.org/A039940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039940:
   For each n ≥ 1, a(n) is the smallest integer k such that
   that the sequence k, 2k, …, nk all contain the digit 9
   in their decimal representations.  */

/* ------------------------------------------------------------------ */
/* Standard headers.   These are permitted in both C11 and C++17       
   The build supplies the integer typedefs seq_t and idx_t.           */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Utility: check whether a decimal number contains digit 9.  This     
   function is deliberately simple and works for any seq_t that       
   supports the usual arithmetic operators.                         */
static bool has9(seq_t x)
{
    if (x == 0) return false;
    while (x > 0)
    {
        if ((x % 10) == 9) return true;
        x /= 10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Global static cache: cache[n] holds a(n) for 1 ≤ n < cache_cap.  
   cache[0] is unused.  next_n is the first index that has not been
   filled yet.  cur_k is the candidate k currently being examined.  */
static seq_t *cache      = NULL;   /* cache[1..cache_cap-1] holds a(1..cache_cap-1) */
static size_t cache_cap  = 0;      /* number of allocated slots (including 0) */
static idx_t   next_n    = 1;      /* first unfilled index               */
static idx_t   cur_k     = 1;      /* candidate k to examine             */

/* ------------------------------------------------------------------ */
/* Ensure that the cache can hold indices up to 'need' (inclusive).  
   The allocation scheme doubles the current size until it exceeds   
   'need', which keeps reallocations logarithmic in the final size.  */
static void ensure_capacity(size_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (cache == NULL) ?
                 (seq_t *)malloc(new_cap * sizeof(seq_t)) :
                 (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache     = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Fills the cache up to at least index 'n_needed'.  It processes     */
/* successive candidate k values, determining for each the largest   */
/* index 'max_n' for which all multiples m·k (1 ≤ m ≤ max_n) contain  */
/* the digit 9.  All n in the interval [next_n, max_n] are set to  k. */
/* After the loop exits, cache[n] is valid for all n ≤ n_needed. */
static void fill_cache_up_to(idx_t n_needed)
{
    while (next_n <= n_needed)
    {
        seq_t k = cur_k;          /* current candidate */
        idx_t m = 1;              /* test successive multiples */

        /* Find the first m such that (m·k) does NOT contain a 9. */
        while (true)
        {
            if (!has9(k * (seq_t)m))
                break;            /* failure at m */
            ++m;
        }
        idx_t max_n = m - 1;      /* maximal n for which k works */

        /* Store k for all uncovered indices up to max_n. */
        ensure_capacity(max_n + 1);
        for (idx_t idx = next_n; idx <= max_n; ++idx)
            cache[idx] = k;

        /* Prepare for next candidate.  next_n now points past max_n. */
        next_n = max_n + 1;
        ++cur_k;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the build.  'n' is 1‑based.  If n≤0 */
/* the function returns 0 (undefined for such indices).  The function  */
/* may be called in any order; the static cache guarantees memoization. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    fill_cache_up_to(n);
    return cache[n];
}

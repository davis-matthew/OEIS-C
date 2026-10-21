/*
 * A069013  a(1)=a(2)=a(3)=1; for n > 3, a(n) = floor(a(n-3) + a(n-2)/a(n-1)).
 * https://oeis.org/A069013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=1; for n>3, a(n)=floor(a(n-3)+a(n-2)/a(n-1)) */

#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  Cache for computed terms.
 *  a_cache[0] holds a(1), a_cache[1] holds a(2), etc.
 *--------------------------------------------------------------------*/
static seq_t *a_cache     = NULL;   /* array of computed terms   */
static size_t a_cache_alloc = 0;    /* memory allocated (number of slots)  */
static size_t a_cache_len   = 0;    /* number of terms already filled      */

/*--- Ensure that the cache can hold at least new_n terms (1‑based index). */
static void a_ensure_cache(idx_t new_n)
{
    if ((size_t)new_n <= a_cache_alloc)
        return;

    /* allocate a little extra for future growth */
    size_t new_size = (size_t)new_n + 10;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) {                /* allocation failed – abort execution */
        exit(1);
    }
    /* zero‑initialize any newly added slots (only safe for trivial types) */
    if (new_size > a_cache_alloc)
        memset(tmp + a_cache_alloc, 0,
               (new_size - a_cache_alloc) * sizeof(seq_t));

    a_cache      = tmp;
    a_cache_alloc = new_size;
}

/*-------------------------------
 *  Return the n‑th term of A069013.
 *  n is 1‑based.  Calls may come in any order.
 *--------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined below offset */

    /* Make sure we have storage for up to n terms */
    a_ensure_cache(n);

    /* Initialise the first three terms if this is the first request */
    if (a_cache_len < 3) {
        a_cache[0] = 1;                /* a(1) */
        a_cache[1] = 1;                /* a(2) */
        a_cache[2] = 1;                /* a(3) */
        a_cache_len = 3;
    }

    /* Compute any missing terms up to index n (1‑based) */
    for (idx_t idx = (idx_t)a_cache_len; idx < n; ++idx) {
        seq_t a_nm3 = a_cache[idx - 3]; /* a(idx-2)  → a(n-3) in 1‑based */
        seq_t a_nm2 = a_cache[idx - 2]; /* a(idx-1)  → a(n-2) */
        seq_t a_nm1 = a_cache[idx - 1]; /* a(idx)    → a(n-1) */
        a_cache[idx] = a_nm3 + (a_nm2 / a_nm1);
    }

    a_cache_len = (size_t)n;            /* now terms up to n are available */
    return a_cache[n - 1];              /* array is 0‑based, n is 1‑based */
}

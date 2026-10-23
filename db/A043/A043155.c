/*
 * A043155  Numbers k such that 0 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043155: numbers k such that 0 and 4 occur juxtaposed in the
   base‑8 representation of k but not in that of k−1.  The function
   returns the n‑th such number (1‑based indexing).  The implementation
   is a simple incremental search with memoisation.  No hard‑coded
   table is used; the search proceeds from k = 1 upwards.  The routine
   works for all supported seq_t types and is efficient enough for
   n up to about 10000. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has04.h"

/* Append val to the dynamic array *buf[.0..size-1], growing it if
   necessary.  The array is reallocated using realloc; we cast the
   returned void* to seq_t* so that the code compiles with arbitrary
   big‑integer types. */
static void append(seq_t **buf, size_t *size, size_t *cap, seq_t val)
{
    if (*size >= *cap)
    {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *buf = (seq_t *)realloc(*buf, *cap * sizeof(seq_t));
    }
    (*buf)[(*size)++] = val;
}

/* Return the n‑th term of the sequence (n >= 1).  The function is
   memoised: previously found terms are stored in a static array. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array holding found terms           */
    static size_t cache_size = 0;    /* number of terms currently cached    */
    static size_t cache_cap = 0;     /* current capacity of the array       */
    static idx_t next_k = 1;         /* integer to test next               */

    if (n == 0)            /* undefined, but harmless */
        return 0;

    /* If we already have the requested term, return it. */
    if (cache_size >= n)
        return cache[n - 1];

    /* Otherwise extend the cache until it contains n terms. */
    while (cache_size < n)
    {
        seq_t val = (seq_t)next_k;
        bool h_cur = has04(val);
        bool h_prev = has04((seq_t)(next_k - 1ULL));
        if (h_cur && !h_prev)
            append(&cache, &cache_size, &cache_cap, val);
        ++next_k;
    }

    return cache[n - 1];
}

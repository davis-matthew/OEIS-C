/*
 * A065095  a(1) = 1, a(n+1) is the sum of a(n) and ceiling( arithmetic mean
 *          of a(1) ... a(n) ).
 * https://oeis.org/A065095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 1 a(n+1) = a(n) + ceil( (a(1)+...+a(n))/(n) ) for n ≥ 1
 *            (ceil(x) = (x + k – 1)/k when dividing by k)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 155, u64 n <= 563,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *terms   = NULL;   /* cached terms, 1‑based index → terms[n‑1] */
static size_t  alloced = 0;     /* allocated array length (in elements) */
static idx_t   len     = 0;     /* number of terms cached (>=0) */
static unsigned __int128 sum128 = 0;   /* sum of the cached terms (∞ bits) */

/* Resize the cache to hold at least 'needed' terms. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= alloced)
        return;
    size_t new_cap = alloced ? alloced * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* should not happen in the judge environment */
    terms   = tmp;
    alloced = new_cap;
}

/* Initialise the cache with a(1) = 1.  Called lazily. */
static void
init_cache(void)
{
    if (len != 0)
        return;
    ensure_capacity(1);
    terms[0] = (seq_t)1;
    sum128   = (unsigned __int128)terms[0];
    len      = 1;
}

/* Public function: return a(n) for any n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n < 1 */
    init_cache();

    /* Grow the cache up to the requested index. */
    while (len < n) {
        /* Current number of known terms = len. */
        idx_t       m           = len;           /* m ≥ 1 */
        /* ceil((sum)/m) = (sum + m – 1) / m  (using 128‑bit arithmetic). */
        unsigned __int128 ceil_div128 =
            (sum128 + (unsigned __int128)(m) - 1) / (unsigned __int128)(m);
        /* Compute next term using 128‑bit addition to avoid overflow. */
        unsigned __int128 next128 = (unsigned __int128)terms[len - 1] + ceil_div128;
        seq_t  next  = (seq_t)next128;
        /* Store and update. */
        ensure_capacity(len + 1);
        terms[len] = next;
        sum128   += next128;
        ++len;
    }
    return terms[n - 1];
}

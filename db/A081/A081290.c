/*
 * A081290  a(0) = 0, and for n >=1, a(n) = the largest Catalan number <= n.
 * https://oeis.org/A081290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest Catalan number ≤ n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes Catalan numbers on demand.
   The recurrence C_{k+1} = C_k * 2*(2k+1)/(k+2) is used, which
   guarantees exact integer results for all indices.  The list of
   Catalan numbers is grown as large enough to cover the current
   argument.  A binary search then returns the greatest Catalan
   number not exceeding n.  The routine works for any build
   where seq_t is an unsigned integer type (including arbitrary‑precision
   integers). */

#include <stdlib.h>

static seq_t *catalan = NULL;
static idx_t catalan_size = 0;
static idx_t catalan_cap  = 0;

/* Extend the Catalan array until the last stored value exceeds |limit|. */
static void ensure_catalan_upto(seq_t limit)
{
    if (catalan_size == 0) {                 /* initialise the list */
        catalan_cap   = 4;
        catalan       = (seq_t *)malloc(catalan_cap * sizeof(seq_t));
        catalan[0]    = (seq_t)1;             /* C_0 = 1 */
        catalan_size  = 1;
    }

    while (catalan[catalan_size - 1] <= limit)
    {
        idx_t idx  = catalan_size - 1;          /* current k  */
        seq_t last = catalan[catalan_size - 1];

        /* C_{k+1} = C_k * 2*(2k+1)/(k+2) */
        seq_t factor = (seq_t)2 * ((seq_t)(2 * idx + 1));
        seq_t denom  = (seq_t)(idx + 2);
        seq_t next   = last * factor / denom;   /* exact integer division */

        if (catalan_size == catalan_cap) {      /* grow buffer  */
            catalan_cap = catalan_cap * 2;
            catalan = (seq_t *)realloc(catalan,
                                       catalan_cap * sizeof(seq_t));
        }
        catalan[catalan_size++] = next;

        /* If the newly created Catalan number already exceeds the
           desired limit we can stop – it will still be stored for
           future searches. */
        if (next > limit)
            break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t limit = (seq_t)n;
    ensure_catalan_upto(limit);

    /* Binary search for the first Catalan number larger than |n|. */
    idx_t lo = 0, hi = catalan_size;           /* hi exclusive */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (catalan[mid] > limit)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* lo is the first index with catalan[lo] > n; the desired value
       is the one immediately before it. */
    return catalan[lo - 1];
}

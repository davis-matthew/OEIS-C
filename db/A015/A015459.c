/*
 * A015459  q-Fibonacci numbers for q=2, scaling a(n-2).
 * https://oeis.org/A015459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=a(n-1)+2^(n-2)*a(n-2) for n>=2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of the sequence values and
   extends it only when a larger index is requested.  It uses O(n) time
   and O(n) space, and works for any unsigned integer type seq_t
   (including arbitrary‑precision C++ classes).  All arithmetic is
   performed with type seq_t to keep values exact within the limits
   of the chosen representation. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* storage for sequence values   */
static size_t cap   = 0;         /* allocated capacity (in indices)  */
static idx_t    last = 1;        /* highest index currently computed */
static seq_t    pow2 = 1;        /* holds 2^(i-2) for the next i = last+1 */

seq_t a(idx_t n)
{
    /* trivial base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialise cache on first call */
    if (cap == 0) {
        cap = 2;                      /* need indices 0 and 1 at least */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – fall back to abort */
            abort();
        }
        /* initialise the newly allocated part to zero */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a[i] = a[i-1] + 2^(i-2) * a[i-2] */
        cache[i] = cache[i-1] + pow2 * cache[i-2];
        /* Prepare pow2 for the next index: 2^(i-1) = 2 * 2^(i-2) */
        pow2 = pow2 * (seq_t)2;
    }
    last = n;   /* update the highest computed index */

    return cache[n];
}

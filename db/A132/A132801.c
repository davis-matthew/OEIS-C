/*
 * A132801  a(0)=1; a(n) = Sum_{1<=k<=n, gcd(k,n)=1} a(floor(k/2)), where the
 *          sum is over those positive integers which are <= n and are coprime
 *          to n.
 * https://oeis.org/A132801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A132801
 *  a(0)=1;   a(n) = Sum_{1<=k<=n , gcd(k,n)=1} a(floor(k/2))
 *  Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=4
 *
 *  The routine keeps a cache of all values that have been computed; the
 *  array size is increased only when a larger index is requested.  The
 *  implementation works for any build where seq_t is an unsigned integer
 *  type, including arbitrary‑precision integer classes.  All arithmetic
 *  involving indices is performed in idx_t and then cast to seq_t
 *  when needed for the gcd routine.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* cache for a(0..max_computed) */
static idx_t max_computed = (idx_t)(-1);   /* no values computed yet */

/* initialise cache to hold a(0)=1 */
static void init_cache(void)
{
    if (cache == NULL) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));  /* cast for C++ */
        cache[0] = (seq_t)1;
        max_computed = 0;
    }
}

/* extend the cache up to index n (inclusive) */
static void extend_cache(idx_t n)
{
    if (max_computed >= n) return;

    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            if (gcd((seq_t)k, (seq_t)i) == (seq_t)1) {
                sum += cache[k / 2];
            }
        }
        cache[i] = sum;
    }
    max_computed = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    init_cache();
    extend_cache(n);
    return cache[n];
}

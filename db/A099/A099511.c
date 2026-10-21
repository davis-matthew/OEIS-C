/*
 * A099511  Row sums of triangle A099510, so that a(n) = Sum_{k=0..n}
 *          coefficient of z^k in (1 + 2*z + z^2)^(n-[k/2]), where [k/2] is
 *          the integer floor of k/2.
 * https://oeis.org/A099511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) (Recurrence as
 *            given in A099511), with initial values a(0)=1, a(1)=3, a(2)=6,
 *            a(3)=17
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamic cache for previously computed values */
static seq_t *cache      = NULL;   /* array of computed terms            */
static idx_t   cache_sz  = 0;      /* current allocated size             */
static idx_t   upto      = 0;      /* highest index that has a value     */

/* Ensure the cache can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    if (cache_sz > n) return;              /* already big enough */
    idx_t new_sz = cache_sz ? cache_sz : 8;
    while (new_sz <= n) new_sz <<= 1;       /* grow to at least n+1 */

    /* realloc is generic; cast the void* result to seq_t* */
    seq_t *tmp = (seq_t *)realloc((void *)cache, new_sz * sizeof(seq_t));
    cache      = tmp;
    cache_sz   = new_sz;
}

/* Compute and return the n-th term of A099511 */
seq_t a(idx_t n)
{
    /* Make sure we can hold index n */
    ensure_cache(n);

    /* Initialize the first four terms when needed */
    if (upto == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)17;
        upto = 3;
    }

    /* Compute forward if we haven't reached n yet */
    for (idx_t i = upto + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + cache[i-2] + 2 * cache[i-3] - cache[i-4];
    }
    upto = n > upto ? n : upto;

    return cache[n];
}

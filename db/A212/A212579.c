/*
 * A212579  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          min{|w-x|,|w-y|}=min{|x-y|,|x-z|}.
 * https://oeis.org/A212579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5)
                      +2*a(n-6)+a(n-7)-a(n-8),
   initial values: a(0)=0, a(1)=1, a(2)=8, a(3)=31, a(4)=80,
   a(5)=171, a(6)=308, a(7)=509 */

#include <stddef.h>   /* for size_t */

static seq_t cache[20001];
static idx_t cache_len = 0;

/* Initialise the first eight terms; guaranteed to be called only once. */
static void init_base_terms(void)
{
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)8;
    cache[3] = (seq_t)31;
    cache[4] = (seq_t)80;
    cache[5] = (seq_t)171;
    cache[6] = (seq_t)308;
    cache[7] = (seq_t)509;
    cache_len = 8;
}

/* Ensure that cache contains values up to at least index n. */
static void ensure_cache(idx_t n)
{
    /* For n < 8 only the pre‑initialised constants are needed. */
    if (n < 8) {
        if (cache_len < 8)   /* initialisation not yet performed */
            init_base_terms();
        return;
    }

    /* If already computed up to n, nothing to do. */
    if (cache_len > n)
        return;

    /* Ensure the base terms are present. */
    if (cache_len == 0)
        init_base_terms();

    /* Compute terms from cache_len up to n.  The recurrence requires
       terms down to i-8, but since we start at index 8 we are safe. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-2];
        val -= cache[i-3];
        val -= (seq_t)2 * cache[i-4];
        val -= cache[i-5];
        val += (seq_t)2 * cache[i-6];
        val += cache[i-7];
        val -= cache[i-8];
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

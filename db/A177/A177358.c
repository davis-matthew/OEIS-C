/*
 * A177358  G.f. =
 *          (1+10*x-12*x^2-50*x^3+10*x^4+10*x^5-12*x^6)/(1-8*x-66*x^2+280*x^3+178*x^4-532*x^5-84*x^6+108*x^7).
 * https://oeis.org/A177358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A177358
 *  Generation function:   (1+10*x-12*x^2-50*x^3+10*x^4+10*x^5-12*x^6)
 *                        ------------------------------------------------
 *                         (1-8*x-66*x^2+280*x^3+178*x^4-532*x^5-84*x^6+108*x^7)
 *
 *  Recurrence (for n>7):
 *      a(n) = 8*a(n-1) + 66*a(n-2) - 280*a(n-3) - 178*a(n-4)
 *             + 532*a(n-5) + 84*a(n-6) - 108*a(n-7)
 *
 *  Initial values:
 *      a(1)=1,  a(2)=18,  a(3)=198,  a(4)=2442,  a(5)=27396,
 *      a(6)=322238, a(7)=3676684
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static const seq_t init_vals[7] = {
    1, 18, 198, 2442, 27396, 322238, 3676684
};

static seq_t *cache      = NULL;   /* cache of computed terms */
static size_t cache_cap  = 0;       /* capacity of the cache (in terms) */
static size_t cache_size = 0;       /* number of terms actually stored */

/* Grow the cache to hold at least 'min_cap' terms. */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort(); /* allocation failed */
    cache      = new_buf;
    cache_cap  = new_cap;
}

/* Make sure the first 7 terms are present in the cache.  They are
 * stored starting at cache[0] (corresponding to a(1)).  After the call
 * cache_size is at least 7. */
static void ensure_initial_terms(void)
{
    if (cache_size >= 7)
        return;

    for (size_t i = cache_size; i < 7; ++i)
        cache[i] = init_vals[i];

    if (cache_size < 7)
        cache_size = 7;
}

/*  Sequence A177358:  a(n) for n >= 1                          */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    size_t req = (size_t)n;              /* safe conversion: n <= 10000 usually */

    /* ensure we have storage for the requested term */
    ensure_cache_cap(req);

    /* if we are still missing the first 7 terms, create them */
    ensure_initial_terms();

    /* If the term is already computed, return it */
    if (req <= cache_size)
        return cache[req - 1];

    /* Compute missing terms up to req using the recurrence. */
    for (size_t i = cache_size + 1; i <= req; ++i) {
        seq_t t =
            (seq_t)8 * cache[i - 2]   /* a(i-1)   */
          + (seq_t)66 * cache[i - 3]  /* a(i-2)   */
          - (seq_t)280 * cache[i - 4] /* a(i-3)   */
          - (seq_t)178 * cache[i - 5] /* a(i-4)   */
          + (seq_t)532 * cache[i - 6] /* a(i-5)   */
          + (seq_t)84 * cache[i - 7]  /* a(i-6)   */
          - (seq_t)108 * cache[i - 8];/* a(i-7)   */
        cache[i - 1] = t;
    }
    cache_size = req;                   /* we now have terms up to req */

    return cache[req - 1];
}

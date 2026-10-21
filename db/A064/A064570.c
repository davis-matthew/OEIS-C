/*
 * A064570  Binomial transform of (2n)!.
 * https://oeis.org/A064570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*(2*k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence:
   a(0) = 1, a(1) = 3,
   a(n) = 1 + 2*n*(2*n-1)*a(n-1) - 2*n*(2*n-2)*a(n-2)   (n >= 2)
*/
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* array of computed terms      */
static size_t cache_cap = 0;         /* capacity of cache array      */
static size_t max_computed = (size_t)-1; /* highest index that has been computed */

/* Ensure that cache[] has room for indices up to need (inclusive). */
static void ensure_capacity(size_t need)
{
    if (cache_cap > need) return;
    size_t old_cap = cache_cap;
    size_t new_cap = old_cap ? old_cap << 1 : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
    /* Zero the newly allocated part so that uncomputed entries are 0. */
    if (old_cap < new_cap)
        memset((char *)cache + old_cap * sizeof(seq_t),
               0, (new_cap - old_cap) * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;   /* n will be small enough for size_t */

    /* Initialize the cache with the first term. */
    if (cache == NULL) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Compute up to requested index if necessary. */
    if (N > max_computed) {
        ensure_capacity(N);
        for (size_t i = max_computed + 1; i <= N; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)3;
            } else {
                seq_t a_im1 = cache[i - 1];
                seq_t a_im2 = cache[i - 2];
                seq_t ii = (seq_t)i;

                /* term1 = 2 * i * (2*i-1) * a(i-1) */
                seq_t term1 = (seq_t)2 * ii * ((seq_t)2 * ii - (seq_t)1);
                term1 *= a_im1;

                /* term2 = 2 * i * (2*i-2) * a(i-2) */
                seq_t term2 = (seq_t)2 * ii * ((seq_t)2 * ii - (seq_t)2);
                term2 *= a_im2;

                cache[i] = (seq_t)1 + term1 - term2;
            }
        }
        max_computed = N;
    }

    return cache[N];
}

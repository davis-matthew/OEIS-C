/*
 * A212753  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w<R, x<R, y>R, z>R, where R =
 *          max{w,x,y,z} - min{w,x,y,z}.
 * https://oeis.org/A212753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6) with
 *            a(0)=0, a(1)=12, a(2)=69, a(3)=228, a(4)=568, a(5)=1192
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of elements currently stored */

/* Compute all terms up to n (inclusive) and store them in cache */
static void
ensure_up_to(idx_t n)
{
    if (n < cache_len) return;          /* already computed */

    /* Allocate / grow cache */
    size_t new_len = (size_t)(n + 1);
    host:
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* Handle allocation failure */
    if (!cache) abort();

    /* Base values for indices 0..5 */
    static const seq_t base_vals[6] = {
        (seq_t)0,
        (seq_t)12,
        (seq_t)69,
        (seq_t)228,
        (seq_t)568,
        (seq_t)1192
    };

    /* Compute missing values sequentially */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 6) {
            val = base_vals[i];
        } else {
            /* recurrence: a(i) = 4*a(i-1) - 5*a(i-2) + 5*a(i-4) - 4*a(i-5) + a(i-6) */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];

            val = (seq_t)4 * a1 + a6 + (seq_t)5 * a4;
            val -= (seq_t)5 * a2;
            val -= (seq_t)4 * a5;
        }
        cache[i] = val;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}

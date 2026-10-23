/*
 * A027166  a(n) = Sum_{0<=j<=i<=n} A027157(i, j).
 * https://oeis.org/A027166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+5*a(n-2)-12*a(n-3)+9*a(n-4)-6*a(n-5)+3*a(n-6)
 *            with a(0) = 1, a(1) = 4, a(2) = 14, a(3) = 36, a(4) = 103, a(5)
 *            = 248
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t cache_len = 0;      /* number of values currently stored */

/* Ensure that cache contains values up to n (inclusive).  Values are
   computed using the recurrence.  The routine is safe for any order of
   calls and grows the cache lazily. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already have it */

    /* grow cache to n+1 entries */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();           /* allocation failure */

    /* base values for n = 0..5 */
    const seq_t base[6] = { 1, 4, 14, 36, 103, 248 };

    /* compute missing terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i < 6) {
            new_cache[i] = base[i];
            continue;
        }
        /* compute positive and negative parts separately to avoid
           underflow in unsigned arithmetic */
        seq_t pos = new_cache[i-1] * ((seq_t)2);
        pos += new_cache[i-2] * ((seq_t)5);
        pos += new_cache[i-4] * ((seq_t)9);
        pos += new_cache[i-6] * ((seq_t)3);

        seq_t neg = new_cache[i-3] * ((seq_t)12);
        neg += new_cache[i-5] * ((seq_t)6);

        new_cache[i] = pos - neg;   /* pos >= neg, so this is safe */
    }

    cache = new_cache;
    cache_len = new_len;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

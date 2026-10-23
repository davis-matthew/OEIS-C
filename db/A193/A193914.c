/*
 * A193914  a(1)=1; a(n+1) = Reverse((1 + a(n))^2).
 * https://oeis.org/A193914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; a(n+1) = reverse((1 + a(n))^2) */

#include <stddef.h>
#include <stdlib.h>

/* Reverse the decimal digits of a seq_t value. */
static seq_t rev_decimal(seq_t x)
{
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0) {
        seq_t digit = x % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        x = x / (seq_t)10;
    }
    return rev;
}

/* Return the nth term of OEIS sequence A193914 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;   /* offset is 1; return 0 for invalid indices */
    }

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated number of elements */
    static size_t computed = 0;        /* number of terms already computed */

    /* Grow cache if needed. */
    if ((size_t)n > cache_size) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            return (seq_t)0;   /* allocation failed */
        }
        cache = tmp;
        cache_size = new_cap;
    }

    /* Initialize first term if not done yet. */
    if (computed == 0) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        computed = 1;
    }

    /* Compute missing terms up to n. */
    for (size_t i = computed; i < (size_t)n; ++i) {
        seq_t tmp = cache[i - 1] + (seq_t)1;   /* 1 + a(i) */
        seq_t sq = tmp * tmp;                 /* (1 + a(i))^2 */
        cache[i] = rev_decimal(sq);           /* reverse decimal digits */
    }
    computed = (size_t)n;

    return cache[n - 1];
}

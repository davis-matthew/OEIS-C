/*
 * A229633  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly two
 *          mistakes, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 159*a(n-2) + 511*a(n-3) - 636*a(n-4) * +
 *            336*a(n-5) - 64*a(n-6), n>9 * Initial values: a(1)=3, a(2)=122,
 *            a(3)=1109, a(4)=10227, a(5)=89331, * a(6)=747299, a(7)=6049298,
 *            a(8)=47723226, a(9)=368769958
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, abort */

static const seq_t init_vals[9] = {
    (seq_t)3,          (seq_t)122,        (seq_t)1109,
    (seq_t)10227,      (seq_t)89331,      (seq_t)747299,
    (seq_t)6049298,    (seq_t)47723226,   (seq_t)368769958
};

static seq_t *cache = NULL;    /* cache[0] holds a(1) */
static size_t cache_len = 0;   /* number of entries currently stored */

/* Ensure that cache holds values up to (and including) index n (1‑based). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;   /* already computed */

    size_t new_len = (size_t)n;

    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        abort();    /* Out‑of‑memory: terminate */
    }
    cache = new_cache;
    cache_len = new_len;

    /* On initial allocation or if we didn't have the first 9 values, set them. */
    if (cache_len >= 9) {
        for (size_t i = 0; i < 9; ++i) {
            cache[i] = init_vals[i];
        }
    } else {
        /* cache_len < 9, copy as many initial values as we can. */
        for (size_t i = 0; i < cache_len; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* Now compute further terms using the recurrence. */
    for (idx_t k = 10; k <= n; ++k) {
        /* cache indices are zero‑based: a(k) is at cache[k-1] */
        seq_t a1 = cache[k - 2]; /* a(k-1) */
        seq_t a2 = cache[k - 3]; /* a(k-2) */
        seq_t a3 = cache[k - 4]; /* a(k-3) */
        seq_t a4 = cache[k - 5]; /* a(k-4) */
        seq_t a5 = cache[k - 6]; /* a(k-5) */
        seq_t a6 = cache[k - 7]; /* a(k-6) */

        seq_t val = (seq_t)21 * a1
                    - (seq_t)159 * a2
                    + (seq_t)511 * a3
                    - (seq_t)636 * a4
                    + (seq_t)336 * a5
                    - (seq_t)64  * a6;

        cache[k - 1] = val;
    }
}

/* Public API: compute a(n), n >= 1 */
seq_t
a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* undefined for n < 1 */
    }
    ensure_cache(n);
    return cache[n - 1];
}

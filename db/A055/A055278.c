/*
 * A055278  Number of rooted trees with n nodes and 3 leaves.
 * https://oeis.org/A055278
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence derived from the generating function
 *  G(x) = x^4*(x^3+x^2+1)/((1-x^2)*(1-x^3)*(1-x)^3)
 * The linear recurrence (order 8) is:
 *   a[n] = m[n]
 *          +3*a[n-1]      -2*a[n-2] - a[n-3]
 *          + a[n-5] +2*a[n-6]-3*a[n-7] + a[n-8]
 * where m[n] = 1 for n = 4,6,7 and 0 otherwise.
 * Cache results for fast repeated calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL; /* values of a(n) */
static size_t  cap    = 0;    /* allocated capacity */
static size_t  len    = 0;    /* number of computed values (len-1 is max n computed) */

seq_t a(idx_t n)
{
    /* No need to compute for n < 4 (outside the official offset) */
    if (n < 4) return 0;

    /* Ensure cache can hold indices up to n */
    size_t needed_cap = (size_t)(n + 1);
    if (cap < needed_cap) {
        size_t new_cap = needed_cap;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure – abort */
        /* Zero-fill newly allocated part */
        if (new_cap > cap) {
            size_t i = cap;
            size_t d = new_cap - cap;
            memset(tmp + i, 0, d * sizeof(seq_t));
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first non‑computed indices (0..3) to 0 if needed */
    if (len < 4) {
        size_t start = len;
        size_t end   = 4;
        for (size_t i = start; i < end && i <= (size_t)n; ++i)
            cache[i] = 0;
        len = 4;
    }

    /* Compute values from the first missing index up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t pos = 0; /* positive contributions */
        seq_t neg = 0; /* negative contributions */

        /* m[i] */
        if (i == 4 || i == 6 || i == 7) pos += (seq_t)1;

        if (i >= 1) pos += (seq_t)3 * cache[i - 1];
        if (i >= 2) neg += (seq_t)2 * cache[i - 2];
        if (i >= 3) neg += (seq_t)1 * cache[i - 3];
        if (i >= 5) pos += (seq_t)1 * cache[i - 5];
        if (i >= 6) pos += (seq_t)2 * cache[i - 6];
        if (i >= 7) neg += (seq_t)3 * cache[i - 7];
        if (i >= 8) pos += (seq_t)1 * cache[i - 8];

        cache[i] = pos - neg;   /* guaranteed non‑negative */
    }
    len = (size_t)n + 1;
    return cache[n];
}

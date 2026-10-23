/*
 * A143848  Let x(1) = 1 and x(n) = x(n-1)^2 + i for n>1; a(n) is the real
 *          part of x(n).
 * https://oeis.org/A143848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  x(1) = 1 + 0i,
 * x(n) = x(n–1)^2 + i    (n>1);
 * a(n) = Re(x(n)).  a(1)=1, a(2)=1, a(3)=0, a(4)=–9, a(5)=80, … */

#include <stdlib.h>

/* static cache for real and imaginary parts (indices 0…n, 0 unused) */
static seq_t *re_cache = NULL;  /* real part */
static seq_t *im_cache = NULL;  /* imaginary part */
static idx_t   cap = 0;         /* allocated array size (indices 0…cap-1) */
static idx_t   last = 0;        /* highest index computed */

/* Return a(n) for any n ≥ 1.  Works for arbitrary call order and
 * uses memoisation to avoid recomputation. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1 – inputs <1 are undefined. */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache can hold indices up to n.  We allocate n+1
     * slots so that indices 1…n are valid (index 0 is unused). */
    if (cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        re_cache = (seq_t *)realloc(re_cache, new_cap * sizeof(seq_t));
        im_cache = (seq_t *)realloc(im_cache, new_cap * sizeof(seq_t));
        if (!re_cache || !im_cache)
            return (seq_t)0;                /* allocation failure handled elsewhere */

        /* Zero entries that have not yet been written. */
        for (idx_t i = cap; i < new_cap; ++i) {
            re_cache[i] = 0;
            im_cache[i] = 0;
        }
        cap = new_cap;
    }

    /* Initialise the base case once. */
    if (last < 1) {
        re_cache[1] = (seq_t)1;   /* real part of x(1) */
        im_cache[1] = (seq_t)0;   /* imaginary part of x(1) */
        last = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a_prev = re_cache[i - 1];
        seq_t b_prev = im_cache[i - 1];

        /* a_i = a_prev^2 − b_prev^2 */
        seq_t a_new = a_prev * a_prev - b_prev * b_prev;

        /* b_i = 2·a_prev·b_prev + 1 */
        seq_t prod = a_prev * b_prev;
        seq_t b_new = prod + prod + (seq_t)1;

        re_cache[i] = a_new;
        im_cache[i] = b_new;
    }
    last = n;
    return re_cache[n];
}

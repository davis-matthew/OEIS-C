/*
 * A107363  Expansion of (1 - x)*(1 + x)^2*(1 + x^2)*(1 - x^2 + 2*x^3 + x^4) /
 *          ((1 - x^2 - x^4)*(1 + x^2 + 2*x^4 - x^6 + x^8)).
 * https://oeis.org/A107363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-6) + a(n-12) for n > 11 with initial values (n = 0
 *            .. 11) given below
 * Range:     i8 n <= 21, i16 n <= 43, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache of computed terms (dynamically grown).  The array is
   allocated on the first call and grows as needed. */
static seq_t *a_arr = NULL;   /* pointer to array of seq_t */
static idx_t  a_size = 0;     /* number of elements stored so far */

/* Initial terms (n = 0 .. 11).  These are copied into the cache
   at the first call and are never modified. */
static const seq_t a_init[12] = {
    (seq_t)1,  (seq_t)1,  (seq_t)-1, (seq_t)1, (seq_t)2, (seq_t)0,
    (seq_t)5,  (seq_t)3,  (seq_t)-7, (seq_t)3, (seq_t)8, (seq_t)0
};

seq_t a(idx_t n)
{
    /* Initialise the cache on first use.  Use a cast to satisfy
       both C and C++ compiler requirements. */
    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc(12 * sizeof(seq_t));
        memcpy(a_arr, a_init, 12 * sizeof(seq_t));
        a_size = 12;
    }

    /* If the requested term is already cached, just return it. */
    if (n < a_size)
        return a_arr[n];

    /* Guard against unsigned underflow when n < 12 (already handled
       above) – this check is not strictly needed but keeps the logic
       clear. */
    if (n < 12) {
        return a_init[n];
    }

    /* Grow the cache up to and including n. */
    idx_t new_size = n + 1;
    a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    for (idx_t i = a_size; i <= n; ++i) {
        seq_t val = a_arr[i - 6];          /* a(i-6) */
        val = val * (seq_t)4;              /* 4*a(i-6) */
        val = val + a_arr[i - 12];         /* + a(i-12) */
        a_arr[i] = val;
    }
    a_size = new_size;
    return a_arr[n];
}

/*
 * A061000  a(n) = Sum_{k=1..n} A060999(k) * k^3.
 * https://oeis.org/A061000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} round(k^3/9) * k^3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* here round(x) returns the nearest integer to x   */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Prefix sums of a(k).  prefix[0] = 0. */
    static seq_t *prefix = NULL;
    static idx_t pref_size = 0;   /* highest n computed so far */
    static size_t alloc_size = 0; /* allocated array size (>= pref_size+1) */

    if (n == 0) return (seq_t)0; /* behaviour below offset allowed to be arbitrary */

    /* Ensure capacity for index n */
    if (n > pref_size) {
        /* Grow array: double size until it fits n+1 */
        size_t req = (size_t)n + 1;
        if (alloc_size < req) {
            size_t new_size = alloc_size ? alloc_size * 2 : 16;
            if (new_size < req) new_size = req;
            seq_t *newp = (seq_t *)realloc(prefix, new_size * sizeof(seq_t));
            if (!newp) return (seq_t)0; /* allocation failed, return 0 */
            prefix = newp;
            alloc_size = new_size;
        }
        /* Compute new terms up to n */
        for (idx_t i = pref_size + 1; i <= n; ++i) {
            seq_t k = (seq_t)i;
            seq_t k3 = k * k * k;                 /* k^3 */
            seq_t div = k3 / (seq_t)9;             /* floor(k^3/9)   */
            seq_t rem = k3 % (seq_t)9;             /* remainder      */
            if ((rem << 1) >= (seq_t)9)      /* round up if rem*2 >= 9 */
                div += (seq_t)1;
            seq_t term = div * k3;                /* round(k^3/9) * k^3 */
            prefix[i] = prefix[i - 1] + term;     /* prefix sum */
        }
        pref_size = n;
    }
    return prefix[n];
}

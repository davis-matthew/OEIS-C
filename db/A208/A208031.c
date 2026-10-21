/*
 * A208031  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+30*a(n-2)-64*a(n-3)-157*a(n-4)
 *            +303*a(n-5)+101*a(n-6)-262*a(n-7)-23*a(n-8) +64*a(n-9)-4*a(n-11)
 *            Initial values: a(1)=19, a(2)=361, a(3)=1995, a(4)=17119,
 *            a(5)=123709, a(6)=955073, a(7)=7184755, a(8)=54606513,
 *            a(9)=413322903, a(10)=3133722193, a(11)=23742848381. The
 *            function can be...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are provided by the build environment. */

/* Static array holding the first 11 terms. Index 0 is unused. */
static const seq_t _init[12] = {
    0ULL,
    19ULL,
    361ULL,
    1995ULL,
    17119ULL,
    123709ULL,
    955073ULL,
    7184755ULL,
    54606513ULL,
    413322903ULL,
    3133722193ULL,
    23742848381ULL
};

/* The main function. */
seq_t a(idx_t n)
{
    /* Sanity check: sequence is defined for n >= 1. */
    if (n < 1) return 0;

    /* Return a known value directly if requested. */
    if (n <= 11) return _init[(size_t)n];

    /* Use a sliding window of the last 11 values. */
    seq_t r[11];
    for (int i = 0; i < 11; ++i)
        r[i] = _init[i+1];   /* r[0] = a(1), r[10] = a(11) */

    seq_t cur = 0;

    /* Iterate from a(12) up to a(n). */
    for (idx_t k = 12; k <= n; ++k) {
        /* Compute the next term using the recurrence. */
        // Terms are added using seq_t arithmetic; it is assumed that
        // seq_t supports the usual +, -, * operations with small
        // integer constants.
        cur =  r[10] * (seq_t)5
             + r[9]  * (seq_t)30
             - r[8]  * (seq_t)64
             - r[7]  * (seq_t)157
             + r[6]  * (seq_t)303
             + r[5]  * (seq_t)101
             - r[4]  * (seq_t)262
             - r[3]  * (seq_t)23
             + r[2]  * (seq_t)64
             - r[0]  * (seq_t)4;

        /* Shift the window for the next step. */
        for (int i = 0; i < 10; ++i)
            r[i] = r[i + 1];
        r[10] = cur;   /* now r[10] holds a(k) */
    }

    return cur;
}

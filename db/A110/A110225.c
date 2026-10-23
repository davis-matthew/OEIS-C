/*
 * A110225  Expansion of (-1+3*x+2*x^2-8*x^3+3*x^5-2*x^6-2*x^7+x^8) /
 *          ((x-1)*(x+1)*(x^2-2*x-1)*(x^2+2*x-1)).
 * https://oeis.org/A110225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = 7*a(n-2) - 7*a(n-4) + a(n-6)   for n > 8
 * Initial values:
 *   a(0)= 1, a(1)=-3, a(2)= 5, a(3)=-13,
 *   a(4)=28, a(5)=-73, a(6)=164, a(7)=-421, a(8)=956
 * The function computes a(n) for any n ≥ 0, memoizing previous
 * results so that calls in arbitrary order remain efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* the first nine exact values */
    static const seq_t init[9] = {
        1, -3, 5, -13, 28, -73, 164, -421, 956
    };

    /* memoisation buffer and its current size */
    static seq_t *buf   = NULL;
    static idx_t  bufsz = 0;   /* number of elements stored */

    /* For n < 9 we can return directly */
    if (n < 9) {
        return init[n];
    }

    /* Ensure we have space for indices up to n */
    if (bufsz <= n) {
        idx_t oldsz = bufsz;
        idx_t newsz = n + 1;               /* total needed entries */
        /* realloc returns void* in C++; cast to seq_t* */
        seq_t *tmp = (seq_t *)realloc(buf, (size_t)newsz * sizeof(seq_t));
        if (!tmp) { abort(); }             /* out of memory */
        buf = tmp;
        /* initialise new elements to zero (not strictly required) */
        for (idx_t i = oldsz; i < newsz; ++i) {
            buf[i] = 0;
        }
        bufsz = newsz;

        /* compute all missing terms between oldsz and n */
        for (idx_t k = oldsz; k <= n; ++k) {
            if (k < 9) {                    /* safety, though k>=9 here */
                buf[k] = init[k];
            } else {
                /* a(k) = 7*a(k-2) - 7*a(k-4) + a(k-6) */
                buf[k] = 7 * buf[k-2] - 7 * buf[k-4] + buf[k-6];
            }
        }
    }

    return buf[n];
}

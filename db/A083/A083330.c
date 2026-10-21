/*
 * A083330  a(n) = (3*4^n - 2*3^n + 2^n)/2.
 * https://oeis.org/A083330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*4^n - 2*3^n + 2^n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of powers and sequence values */
    static seq_t *pow2 = NULL;
    static seq_t *pow3 = NULL;
    static seq_t *pow4 = NULL;
    static seq_t *val = NULL;
    static size_t cached = 0;          /* number of stored terms (size of arrays) */
    static idx_t last = (idx_t)-1;     /* highest index currently computed */

    /* Grow the cache if necessary */
    if (cached <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        pow2 = (seq_t*)realloc(pow2, new_size * sizeof(seq_t));
        pow3 = (seq_t*)realloc(pow3, new_size * sizeof(seq_t));
        pow4 = (seq_t*)realloc(pow4, new_size * sizeof(seq_t));
        val = (seq_t*)realloc(val, new_size * sizeof(seq_t));
        cached = new_size;

        /* Initialise the missing terms */
        if (last == (idx_t)-1) {          /* first call */
            pow2[0] = (seq_t)1;
            pow3[0] = (seq_t)1;
            pow4[0] = (seq_t)1;
            val[0] = ( (seq_t)3 * pow4[0] - (seq_t)2 * pow3[0] + pow2[0] ) / 2;
            last = 0;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        pow2[i] = pow2[i-1] * (seq_t)2;
        pow3[i] = pow3[i-1] * (seq_t)3;
        pow4[i] = pow4[i-1] * (seq_t)4;
        val[i] = ( (seq_t)3 * pow4[i] - (seq_t)2 * pow3[i] + pow2[i] ) / 2;
    }
    last = n;
    return val[n];
}

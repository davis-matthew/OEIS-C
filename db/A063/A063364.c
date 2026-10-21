/*
 * A063364  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          91 ).
 * https://oeis.org/A063364
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10) for n > 13, with a(2) = –1,
 *            a(3) = 261, a(4) = 582, a(5) = 900, a(6) = 1218, a(7) = 1536,
 *            a(8) = 1854, a(9) = 2172, a(10)=2490, a(11)=2808, a(12)=3126,
 *            a(13)=3448. The sequence is indexed as a(2) being the first term
 * Range:     i8 n <= 2, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = 0;  /* largest n that has been computed */

    /* Initialisation with known terms */
    if (seq == NULL) {
        capacity = 32;                 /* start with room for indices 0..31 */
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq)
            return 0; /* allocation failure, unlikely */
        /* Zero initial values */
        for (size_t i = 0; i < capacity; ++i)
            seq[i] = 0;

        /* known values up to a(13) */
        seq[2]  = (seq_t)-1;
        seq[3]  = (seq_t)261;
        seq[4]  = (seq_t)582;
        seq[5]  = (seq_t)900;
        seq[6]  = (seq_t)1218;
        seq[7]  = (seq_t)1536;
        seq[8]  = (seq_t)1854;
        seq[9]  = (seq_t)2172;
        seq[10] = (seq_t)2490;
        seq[11] = (seq_t)2808;
        seq[12] = (seq_t)3126;
        seq[13] = (seq_t)3448;

        max_computed = 13;
    }

    /* Grow array if needed */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        /* initialise new slots to 0 */
        for (size_t i = capacity; i < new_cap; ++i)
            tmp[i] = 0;
        seq = tmp;
        capacity = new_cap;
    }

    /* If n already computed, return it */
    if (n <= max_computed)
        return seq[(size_t)n];

    /* Compute forward up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i > 13) {
            seq[(size_t)i] = seq[(size_t)(i - 4)] +
                             seq[(size_t)(i - 6)] -
                             seq[(size_t)(i - 10)];
        } else {
            /* for safety; should not happen because i > 13 */
            seq[(size_t)i] = 0;
        }
    }
    max_computed = n;

    return seq[(size_t)n];
}

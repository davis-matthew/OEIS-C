/*
 * A021174  Expansion of 1/((1-x)(1-2x)(1-6x)(1-7x)).
 * https://oeis.org/A021174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(2)=173, a(3)=1592; * for n>3
 *            a(n)=16*a(n-1)-83*a(n-2)+152*a(n-3)-84*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL; /* cache of computed a(k) */
    static size_t cap   = 0;    /* allocated capacity of seq   */
    static size_t len   = 0;    /* number of valid elements */

    /* grow capacity if needed */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialise the first four terms on first use */
    if (len == 0) {
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)16;
        seq[2] = (seq_t)173;
        seq[3] = (seq_t)1592;
        len = 4;
    }

    /* compute missing terms up to n */
    if (len <= (size_t)n) {
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t t1 = (seq_t)16 * seq[i-1];
            seq_t t2 = (seq_t)83 * seq[i-2];
            seq_t t3 = (seq_t)152 * seq[i-3];
            seq_t t4 = (seq_t)84 * seq[i-4];
            seq[i] = t1 - t2 + t3 - t4;
        }
        len = (size_t)n + 1;
    }

    return seq[n];
}

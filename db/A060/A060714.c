/*
 * A060714  a(n) = a(n-1) + a(n-1 minus the number of terms of the same parity
 *          as n so far).
 * https://oeis.org/A060714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-1 - k) where k is the number of a(i) with
 *            the same parity as n among i = 1..n-1. a(1) = 1, offset = 1
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed sequence values */
    static seq_t *seq = NULL;
    static idx_t cap   = 0;       /* allocated capacity */
    static idx_t made  = 0;       /* highest index computed */
    static idx_t evCnt = 0;       /* number of even-valued terms so far */
    static idx_t odCnt = 0;       /* number of odd-valued terms so far */

    if (n == 0) return 0;          /* safety; offset is 1 */

    /* grow the buffer if necessary */
    if (n > cap) {
        /* grow a bit larger than needed to reduce reallocs */
        idx_t newCap = n + 16;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (newCap + 1));
        if (!seq) abort();        /* unlikely, but guarantees defined behaviour */
        cap = newCap;
    }

    /* compute new terms up to n */
    for (idx_t i = made + 1; i <= n; ++i) {
        if (i == 1) {
            seq[1] = (seq_t)1;
            /* initialise parity counters */
            odCnt = 1;
            evCnt = 0;
        } else {
            /* parity of current index i (1 = odd, 0 = even) */
            idx_t p = i & (idx_t)1;
            /* pick the right counter */
            idx_t k = (p) ? odCnt : evCnt;
            /* index of the term to add */
            idx_t j = (i - (idx_t)1) - k;
            /* the recurrence */
            seq_t val = seq[i - (idx_t)1] + seq[j];
            seq[i] = val;
            /* update parity counters */
            if ((val & (seq_t)1) != 0)
                ++odCnt;
            else
                ++evCnt;
        }
    }
    made = n;
    return seq[n];
}

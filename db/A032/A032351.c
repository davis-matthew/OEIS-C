/*
 * A032351  Number of permutations of length n which avoid the patterns 2143,
 *          1324 (smooth permutations); or avoid the patterns 1342, 2431; etc.
 * https://oeis.org/A032351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Recurrence a(0)=1, a(1)=1, a(2)=2 a(n) = 6*a(n-1) - 8*a(n-2) +
 *            4*a(n-3) - 2*C(n-3) (n>=3) where C(k) is the k‑th Catalan
 *            number. The recurrence follows from the generating function
 *            (1-5*x+3*x^2+x^2*sqrt(1-4*x))/(1-6*x+8*x^2-4*x^3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t computed = 0;
    static size_t cap = 0;

    /* grow the array to hold at least n+1 entries */
    if ((size_t)(n + 1) > cap) {
        seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
        if (!seq) abort();          /* memory error is fatal */
        cap = n + 1;
    }

    /* compute missing values */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0)          seq[0] = (seq_t)1;
        else if (i == 1)     seq[1] = (seq_t)1;
        else if (i == 2)     seq[2] = (seq_t)2;
        else {
            seq_t t1 = seq[i - 1];
            t1 *= (seq_t)6;

            seq_t t2 = seq[i - 2];
            t2 *= (seq_t)8;

            seq_t t3 = seq[i - 3];
            t3 *= (seq_t)4;

            seq_t cat = catalan(i - 3);
            cat *= (seq_t)2;

            /* a[i] = t1 - t2 + t3 - cat */
            seq[i] = t1 - t2 + t3 - cat;
        }
    }
    computed = n + 1;
    return seq[n];
}

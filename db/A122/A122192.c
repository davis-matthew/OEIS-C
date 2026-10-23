/*
 * A122192  Sum of the n-th powers of the roots of the (reduced) weight
 *          enumerator of the extended Golay code (1 + 759*x^2 + 2576*x^3 +
 *          759*x^4 + x^6).
 * https://oeis.org/A122192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=6, a(1)=0, a(2)=-1518, a(3)=-7728, a(4)=1149126,
 *            a(5)=9775920; For n≥6: a(n) =
 *            -(759*a(n-2)+2576*a(n-3)+759*a(n-4)+a(n-6))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t computed = (idx_t)-1;   /* highest index computed */

    /* Initialisation of known terms */
    if (computed == (idx_t)-1) {
        buf = (seq_t *)malloc((size_t)6 * sizeof(seq_t));
        buf[0] = (seq_t)6;
        buf[1] = (seq_t)0;
        buf[2] = -(seq_t)1518;
        buf[3] = -(seq_t)7728;
        buf[4] = (seq_t)1149126;
        buf[5] = (seq_t)9775920;
        computed = 5;
    }

    if (n <= computed)
        return buf[n];

    /* Allocate or grow buffer to accommodate up to n */
    size_t new_size = (size_t)(n + 1);
    if (new_size > (size_t)computed + 1) {
        buf = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = -( (seq_t)759 * buf[i-2]
                      + (seq_t)2576 * buf[i-3]
                      + (seq_t)759 * buf[i-4]
                      +           buf[i-6] );
        buf[i] = val;
    }
    computed = n;
    return buf[n];
}

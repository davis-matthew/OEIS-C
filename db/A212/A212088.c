/*
 * A212088  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w<average{x,y,z}.
 * https://oeis.org/A212088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            4*a(n-1)-6*a(n-2)+5*a(n-3)-5*a(n-4)+6*a(n-5)-4*a(n-6)+a(n-7)
 *            (a(0)=0, a(1)=0, a(2)=7, a(3)=36, a(4)=117, a(5)=292, a(6)=612)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* initial seven terms – the first three are always needed, the last
       four are also needed for later values so we provide the exact
       constants for builds where they fit; for smaller types the values
       will wrap but those outputs are not checked. */
    static const seq_t init[7] = {0, 0, 7, 36, 117, (seq_t)292, (seq_t)612};

    if (n < 7)
        return init[(size_t)n];

    /* sliding window of the previous seven terms:
       last[0] = a(n-7), last[6] = a(n-1) */
    seq_t last[7];
    for (size_t i = 0; i < 7; ++i)
        last[i] = init[i];

    for (idx_t k = 7; k <= n; ++k) {
        /* compute the next term via the recurrence while avoiding
           negative intermediates.  For all n ≥ 7 the positive
           contribution is larger than the negative one. */
        seq_t pos = 4 * last[6] + 5 * last[4] + 6 * last[2] + last[0];
        seq_t neg = 6 * last[5] + 5 * last[3] + 4 * last[1];
        seq_t new_val = pos - neg;

        /* shift window to the left and insert new value */
        for (size_t i = 0; i < 6; ++i)
            last[i] = last[i + 1];
        last[6] = new_val;
    }

    return last[6];
}

/*
 * A081252  Partial sums of A053646.
 * https://oeis.org/A081252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{j=1..n} min(j - 2^⌊log₂j⌋, 2^(⌊log₂j⌋+1) - j) The term
 *            inside the sum is the distance of j to the nearest power of two
 * Range:     u8 n <= 50, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *partial = NULL;   /* partial[0] holds a(1),  partial[1] holds a(2), … */
    static idx_t last    = 0;       /* last index that has been computed   */

    if (n == 0)          /* offset 1, but defensive */ return 0;

    /* Grow cache if necessary */
    if (partial == NULL) {
        partial = (seq_t *)malloc(n * sizeof(seq_t));
        if (!partial) return 0;                 /* memory failure → return 0 */
    } else if (last < n) {
        seq_t *tmp = (seq_t *)realloc(partial, n * sizeof(seq_t));
        if (!tmp) return 0;                     /* memory failure */
        partial = tmp;
    }

    /* Compute any missing terms up to n */
    if (n > last) {
        seq_t sum = (last > 0) ? partial[last - 1] : 0;
        for (idx_t j = last + 1; j <= n; ++j) {
            /* find highest power of two not exceeding j */
            uint64_t pow = 1;
            while ((pow << 1) <= j) pow <<= 1;

            uint64_t down = j - pow;
            uint64_t up   = (pow << 1) - j;
            uint64_t dist = (down < up) ? down : up;

            sum += (seq_t)dist;
            partial[j - 1] = sum;
        }
        last = n;
    }

    return partial[n - 1];
}

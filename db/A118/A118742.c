/*
 * A118742  Numbers n for which the expression n!/(n+1) is an integer.
 * https://oeis.org/A118742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n)=A002808(n+1)-1 for n>=1 (i.e., all n≥0 except n=3
 *            for which n+1 is composite)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is always 0 */
        return (seq_t)0;

    idx_t count = 0;     /* number of terms seen so far after a(0) */
    idx_t cand  = 1;     /* candidate n, start after 0 */

    for (;;) {
        if (cand == 3) {     /* special exclusion of n=3 */
            ++cand;
            continue;
        }

        /* Include cand whenever n+1 is composite */
        if (!is_prime(cand + 1)) {
            ++count;
            if (count == n)   /* reached the requested index */
                return (seq_t)cand;
        }
        ++cand;
    }
}

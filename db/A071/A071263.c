/*
 * A071263  Smallest nontrivial composite number beginning with n.
 * https://oeis.org/A071263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest composite integer whose decimal representation
 *            begins with the decimal representation of n, excluding n itself
 *            and the single‐digit suffix n0 (i.e., n followed by one 0). This
 *            is computed by exhaustive search over increasing suffix lengths,
 *            testing each candidate for compositeness
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* pow10 = 10^len of the suffix */
    seq_t pow10 = 10;          /* len = 1 first */
    seq_t start, x;
    while (1) {
        start = (seq_t)n * pow10;   /* n followed by len zeros */

        /* iterate over suffix digits from 0 to pow10-1
           but skip the forbidden case n0 (len==1 && suffix==0) */
        for (seq_t suffix = (pow10 == 10) ? 1 : 0; suffix < pow10; ++suffix) {
            x = start + suffix;
            if (x <= 1) continue;                /* ignore 0,1 */
            if (!isPrimeGeneric(x)) return x;     /* composite found */
        }

        /* increase suffix length: multiply by 10 */
        pow10 *= 10;
    }
}

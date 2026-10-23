/*
 * A131677  a(n) = (Product_{i=1..7} n^i+i) / 7!.
 * https://oeis.org/A131677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Product_{i=1..7} (n^i + i)) / 7!
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 32,
 *            big any n
 * Helpers:   _seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/_seq_gcd.h"

/* Local GCD routine for seq_t values */

seq_t a(idx_t n)
{
    const seq_t FACT = (seq_t)5040;           /* 7!            */
    seq_t divrem = FACT;                     /* Remaining divisor   */
    seq_t prod   = (seq_t)1;                 /* Accumulated product */

    seq_t power = (seq_t)n;                  /* n^i for current i   */

    for (idx_t i = 1; i <= 7; ++i) {
        /* Compute n^i   (for i>1 we have already multiplied in previous step) */
        if (i > 1)
            power *= (seq_t)n;

        seq_t factor = power + (seq_t)i;     /* n^i + i              */

        /* Cancel any common factor with the remaining divisor */
        if (divrem != (seq_t)1) {
            seq_t g = _seq_gcd(factor, divrem);
            if (g != (seq_t)1) {
                factor /= g;
                divrem  /= g;
            }
        }

        prod *= factor;
    }

    /* At this point divrem should be 1; if not, the result is not integral,
       but the definition guarantees integrality. */
    return prod;
}

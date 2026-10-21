/*
 * A089989  a(n) = 5^(n^2+2n+1)*Product_{j=1..n} (25^j-1).
 * https://oeis.org/A089989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^((n+1)^2) * Product_{j=1..n} (25^j - 1), a(0)=5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   pow5_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow5_seq.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* Product_{j=1..n} (25^j - 1) */
    seq_t pow25 = 1;         /* 25^j */

    for (idx_t j = 1; j <= n; ++j) {
        pow25 *= (seq_t)25;
        seq_t term = pow25 - (seq_t)1;
        prod *= term;
    }

    seq_t exp = (seq_t)(n + 1) * (seq_t)(n + 1);  /* (n+1)^2 */
    seq_t pow5 = pow5_seq(exp);

    return prod * pow5;
}

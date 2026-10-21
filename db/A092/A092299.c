/*
 * A092299  a(n) = 4*3^(n^2+2n+1)*Product_{j=1..n} (9^j-1).
 * https://oeis.org/A092299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * 3^(n^2+2n+1) * Product_{j=1..n} (9^j-1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* base part 4 * 3^(n^2+2n+1) = 4 * 3^((n+1)^2) */
    seq_t res = 4;
    seq_t exp = ((seq_t)n + 1) * ((seq_t)n + 1); /* (n+1)^2 */
    seq_t pow3 = big_pow(3, exp);
    res *= pow3;                         /* now res = 4 * 3^((n+1)^2) */

    /* product over j = 1..n of (9^j - 1) */
    if (n == 0) return res;              /* empty product, finished */

    seq_t pow9 = 9;                      /* 9^1 */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = pow9 - 1;           /* (9^j - 1) */
        res *= term;                     /* multiply current term */
        pow9 *= 9;                       /* advance to 9^(j+1) */
    }
    return res;
}

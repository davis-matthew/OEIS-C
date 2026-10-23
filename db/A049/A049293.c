/*
 * A049293  Number of subgroups of index 3 in fundamental group of a closed
 *          surface of characteristic -n.
 * https://oeis.org/A049293
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 6^(n+1)+3^(n+1)-3*2^(n+1)+1 for n >= 0 a(-2) = 0, a(-1) =
 *            0 (offset = -2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle offsets explicitly */
    if (n < 0)
        return (seq_t)0;  /* a(-2) = 0, a(-1) = 0 */

    /* n >= 0: use the closed‑form formula */
    seq_t exp = (seq_t)n + (seq_t)1;           /* exponent = n+1 */
    seq_t pow6 = big_pow((seq_t)6, exp);       /* 6^(n+1) */
    seq_t pow3 = big_pow((seq_t)3, exp);       /* 3^(n+1) */
    seq_t pow2 = big_pow((seq_t)2, exp);       /* 2^(n+1) */

    seq_t three2 = pow2 * (seq_t)3;            /* 3*2^(n+1) */

    seq_t result = pow6 + pow3 + (seq_t)1;     /* 6^(n+1)+3^(n+1)+1 */
    result = result - three2;                 /* subtract 3*2^(n+1) */

    return result;
}

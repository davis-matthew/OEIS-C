/*
 * A066912  Fourth column of the Eulerian triangle A008292 in square array
 *          format.
 * https://oeis.org/A066912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n+3) - (n+4)*3^(n+3) + (1/2)*(n+3)*(n+4)*2^(n+3) -
 *            (1/6)*(n+2)*(n+3)*(n+4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* exponents are the same for 4, 3 and 2: n+3 */
    idx_t k = n + 3;               /* k = n+3 */

    seq_t pow4 = 1;   /* 4^k */
    seq_t pow3 = 1;   /* 3^k */
    seq_t pow2 = 1;   /* 2^k */

    for (idx_t i = 0; i < k; ++i) {
        pow4 *= 4;
        pow3 *= 3;
        pow2 *= 2;
    }

    seq_t result = pow4;                                   /* 4^k */
    result -= (seq_t)(n + 4) * pow3;                       /* -(n+4)*3^k */

    seq_t t1 = ((seq_t)(n + 3) * (n + 4)) / 2;             /* ((n+3)*(n+4))/2 */
    result += t1 * pow2;                                   /* + t1 * 2^k */

    seq_t t2 = ((seq_t)(n + 2) * (n + 3) * (n + 4)) / 6;   /* ((n+2)*(n+3)*(n+4))/6 */
    result -= t2;                                          /* - t2 */

    return result;
}

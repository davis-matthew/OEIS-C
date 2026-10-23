/*
 * A169833  a(n) = 25*2^(n+3) - (198+392*n/3+36*n^2+10*n^3/3).
 * https://oeis.org/A169833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*2^(n+3) - (198 + 392*n/3 + 36*n^2 + 10*n^3/3)
 *            (Reordered as a(n) = 25*2^(n+3) - (198 + 36*n^2 + (392*n +
 *            10*n^3)/3) )
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t sn = n;

    /* n^2 and n^3 */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */

    /* combined numerator for the two terms with division by 3 */
    seq_t num = (392 * sn) + (10 * n3);
    seq_t div = num / 3;         /* guaranteed to be integer */

    /* 36*n^2 */
    seq_t part3 = 36 * n2;

    /* total part to subtract */
    seq_t subs = 198 + part3 + div;

    /* 25 * 2^(n+3) */
    seq_t pow_term = 25 * pow2(n + 3);

    /* final result */
    return pow_term - subs;
}

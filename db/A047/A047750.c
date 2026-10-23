/*
 * A047750  If n mod 2 = 0 then m := n/2 and a(n) =
 *          (3*m)!*(5*m+1)/((m+1)!*(2*m+1)!); otherwise m := (n-1)/2, a(n) =
 *          6*(3*m+2)!/(m!*(2*m+3)!).
 * https://oeis.org/A047750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m) = (3m)! * (5m+1) / ((m+1)! * (2m+1)!) = C(3m, m) * (5m+1)
 *            / ((m+1)*(2m+1)) a(2m+1) = 6 * (3m+2)! / (m! * (2m+3)!) = 6 *
 *            C(3m+2, m) / (2m+3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) /* a(0) = 1 */
        return (seq_t)1;

    if ((n & 1) == 0) {            /* even n = 2m */
        idx_t m = n >> 1;          /* n/2 */

        /* C(3m, m) */
        seq_t comb = binomial(3 * m, m);

        /* numerator: comb * (5m+1) */
        seq_t numer = comb * (5 * m + 1);

        /* denominator: (m+1)*(2m+1) */
        seq_t denom = (seq_t)(m + 1) * (seq_t)(2 * m + 1);

        return numer / denom;
    } else {                       /* odd n = 2m+1 */
        idx_t m = (n - 1) >> 1;    /* (n-1)/2 */

        /* C(3m+2, m) */
        seq_t comb = binomial(3 * m + 2, m);

        /* numerator: comb * 6 */
        seq_t numer = comb * 6;

        /* denominator: 2m+3 */
        seq_t denom = 2 * m + 3;

        return numer / denom;
    }
}

/*
 * A156883  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1} ((k+1)^4
 *          - (k+1)^3)^i ) with T(n, 0) = n!, read by antidiagonals.
 * https://oeis.org/A156883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(m,k) of the square array defined by *
 *            T(n,k)=∏_{j=1..n} ( ∑_{i=0..j-1} ((k+1)^4-(k+1)^3)^i ), * with
 *            the special value T(n,0)=n! . * a(n) is obtained by reading the
 *            array by antidiagonals
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 24, u64 n <= 40, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find diagonal sum d with d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2) / 2) <= n)
        d++;
    /* triangular number of d */
    idx_t tri = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - tri;
    idx_t m = offset;          /* row index */
    idx_t k = d - offset;      /* column index */

    /* compute T(m,k) */
    if (k == 0) {
        seq_t fact = 1;
        for (idx_t i = 2; i <= m; ++i)
            fact *= (seq_t)i;
        return fact;
    }

    seq_t kp1 = (seq_t)(k + 1);
    seq_t kp1_sq = kp1 * kp1;
    seq_t kp1_cu = kp1_sq * kp1;
    seq_t kp1_qu = kp1_cu * kp1;
    seq_t c = kp1_qu - kp1_cu;   /* (k+1)^4 - (k+1)^3 */

    seq_t result = 1;
    for (idx_t j = 1; j <= m; ++j) {
        seq_t power = 1;   /* c^0 */
        seq_t sum = 1;     /* term i = 0 */
        for (idx_t i = 1; i < j; ++i) {
            power *= c;
            sum += power;
        }
        result *= sum;
    }
    return result;
}

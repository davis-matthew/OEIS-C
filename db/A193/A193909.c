/*
 * A193909  Mirror of the triangle A193908.
 * https://oeis.org/A193909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k * ( Σ_{j=k+1}^{r+1} F_j - (k==0 ? 1 : 0) ) where r =
 *            floor( (sqrt(8n+1) - 1) / 2 ), k = n - r*(r+1)/2, F_1 = F_2 = 1,
 *            F_{m} = F_{m-1} + F_{m-2}. Implementation uses only integer
 *            arithmetic and the provided pow2() helper
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* Find the row r: largest r with r*(r+1)/2 <= n   */
    long double d = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while (((unsigned __int128)r * (r + 1)) / 2 > n) r--;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) r++;

    idx_t k = n - r * (r + 1) / 2;          /* column offset in the row (0‑based) */

    /* Compute Fibonacci numbers up to F(r+1) */
    /* r is small (≈150 for n≈10000), so a static array is sufficient */
    seq_t F[200];
    if (r >= 0) {
        F[1] = 1;
        if (r + 1 >= 2) F[2] = 1;
        for (idx_t i = 3; i <= r + 1; ++i) {
            F[i] = F[i - 1] + F[i - 2];
        }
    }

    /* Sum the required Fibonacci segment */
    seq_t fib_sum = 0;
    for (idx_t j = k + 1; j <= r + 1; ++j) {
        fib_sum += F[j];
    }
    if (k == 0) fib_sum -= 1;              /* exclude F_1 when k==0 */

    /* Final value: multiply by 2^k */
    return fib_sum * pow2(k);
}

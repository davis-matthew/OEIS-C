/*
 * A182562  Number of ways to place k non-attacking semi-knights on an n x n
 *          chessboard, sum over all k>=0.
 * https://oeis.org/A182562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n/2+2)^(n+2) * ∏_{j=1}^{n/2-1} F(j+2)^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           if n is even;  a(n) = F((n+1)/2+2)^((n+1)/2) * F((n-1)/2+2)^((n-1)/2) * ∏_{j=1}^{(n-1)/2} F(j+2)^4  */
/*           where F(k) = fib(k)                                                   */
#include "../utils/fib.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is defined for n ≥ 1 */

    seq_t result;

    if ((n & 1) == 0) {           /* n is even, n = 2*m  */
        idx_t m = n >> 1;

        /* F(m+2)^(n+2)  */
        result = big_pow(fib(m + 2), (seq_t)(n + 2));

        /* ∏_{j=1}^{m-1} F(j+2)^4  */
        for (idx_t j = 1; j <= m - 1; ++j) {
            seq_t term = big_pow(fib(j + 2), (seq_t)4);
            result *= term;
        }
    } else {                      /* n is odd, n = 2*m + 1 */
        idx_t m = (n - 1) >> 1;   /* m = (n-1)/2 */

        /* F(m+3)^(m+1)  */
        result = big_pow(fib(m + 3), (seq_t)(m + 1));

        /* F(m+2)^m  */
        result *= big_pow(fib(m + 2), (seq_t)m);

        /* ∏_{j=1}^{m} F(j+2)^4  */
        for (idx_t j = 1; j <= m; ++j) {
            seq_t term = big_pow(fib(j + 2), (seq_t)4);
            result *= term;
        }
    }

    return result;
}

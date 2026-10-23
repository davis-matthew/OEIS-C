/*
 * A014402  Numbers found in denominators of expansion of Airy function Ai(x).
 * https://oeis.org/A014402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 a(2) = 6 a(2*n) = 6 * Π_{k=2..n} (9*(k-1)^2 +
 *            15*(k-1) + 6) for n ≥ 1 a(2*n+1) = Π_{k=1..n} (3*k*(3*k + 1))
 *            for n ≥ 0 (Indices start at 0; a(n) gives the nth term of
 *            A014402.)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    if ((n & 1) == 0) {          /* even index: n = 2*m */
        idx_t m = n >> 1;
        seq_t res = (seq_t)6;    /* a(2) = 6 */
        for (idx_t k = 2; k <= m; ++k) {
            idx_t t = k - 1;
            seq_t factor = (seq_t)9 * t * t + (seq_t)15 * t + (seq_t)6;
            res = res * factor;
        }
        return res;
    } else {                    /* odd index: n = 2*m+1 */
        idx_t m = (n - 1) >> 1;
        seq_t res = (seq_t)1;    /* a(1) = 1 */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t factor = (seq_t)3 * k * ((seq_t)3 * k + (seq_t)1);
            res = res * factor;
        }
        return res;
    }
}

/*
 * A116082  a(n) = C(n,7) + C(n,6) + C(n,5) + C(n,4) + C(n,3) + C(n,2) +
 *          C(n,1).
 * https://oeis.org/A116082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1}^{7} C(n,k)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sum = (seq_t)n;          /* C(n,1) */
    seq_t c   = sum;
    for (idx_t k = 2; k <= 7 && k <= n; ++k) {
        c = c * (((seq_t)n) - (k - 1)) / (seq_t)k;  /* C(n,k) from C(n,k-1) */
        sum += c;
    }
    return sum;
}

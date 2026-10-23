/*
 * A101623  Modular binomial transform of 10^n.
 * https://oeis.org/A101623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (binomial(n-k, k) mod 2) · 10^k * a(0) =
 *            1, a(1) = 1, a(2) = 11, ... * Implementation uses Lucas' theorem
 *            for binomial coefficient * modulo 2: binomial(x, y) ≡ 1 (mod 2)
 *            iff (y & ~x) == 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;          /* sum */
    seq_t pow10   = 1;          /* 10^k, starts with k = 0 */
    idx_t   maxk   = n / 2;     /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t x = n - k;            /* n - k */
        idx_t y = k;                /* k            */
        /* binomial(n-k, k) mod 2 == 1 iff (y & ~x) == 0 */
        if ((y & (~x)) == 0) {
            result += pow10;       /* add 10^k if coefficient is odd */
        }
        pow10 *= 10;                /* next power of 10 */
    }
    return result;
}

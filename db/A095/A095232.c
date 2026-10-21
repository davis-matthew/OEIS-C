/*
 * A095232  a(1) = 1, a(n+1) = floor(a(n)*10^n/n).
 * https://oeis.org/A095232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = floor(a(n)*10^n / n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* a(1) */
    seq_t pow10 = (seq_t)1;   /* 10^k accumulator */

    for (idx_t k = 1; k < n; ++k) {
        pow10 *= (seq_t)10;          /* pow10 = 10^k */
        seq_t prod = a_val * pow10;  /* a(k) * 10^k */
        a_val = prod / k;            /* floor division by k */
    }
    return a_val;
}

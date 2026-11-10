/*
 * A219650  The nonnegative integers n such that there exists a number k with
 *          A034968(n+k)=k.
 * https://oeis.org/A219650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A219651(2*n) = 2*n - (sum of factorial-base digits of
 *            2*n)
 * Range:     u8 n <= 130, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 80 terms through n = 9805 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2;
    seq_t total = m;
    seq_t sum = 0;
    seq_t base = 2;

    while (m > 0) {
        sum += m % base;
        m /= base;
        base++;
    }
    return total - sum;
}

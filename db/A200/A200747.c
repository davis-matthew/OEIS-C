/*
 * A200747  Number of iterations of A034968 required to reach 1.
 * https://oeis.org/A200747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = number of iterations of A034968 (digit sum in
 *            factorial base) needed to reach 1 starting from n
 * Range:     u8 n <= 849, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9329 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A034968_a(seq_t n)
{
    seq_t sum = 0;
    seq_t b = 2;
    while (n > 0) {
        sum += n % b;
        n /= b;
        b++;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t count = 0;
    while (m > 1) {
        m = A034968_a(m);
        count++;
    }
    return count;
}

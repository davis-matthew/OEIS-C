/*
 * A018302  Divisors of 144.
 * https://oeis.org/A018302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 144 in increasing order (sorted)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0 || n > 15) return (seq_t)0;
    seq_t divisors[15];
    size_t idx = 0;
    seq_t p2 = 1;
    for (size_t a = 0; a <= 4; ++a) {
        seq_t p3 = 1;
        for (size_t b = 0; b <= 2; ++b) {
            divisors[idx++] = p2 * p3;
            p3 *= (seq_t)3;
        }
        p2 *= (seq_t)2;
    }
    for (size_t i = 1; i < 15; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j-1] > key) {
            divisors[j] = divisors[j-1];
            --j;
        }
        divisors[j] = key;
    }
    return divisors[n-1];
}

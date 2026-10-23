/*
 * A102390  An odious count.
 * https://oeis.org/A102390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (1+floor(n/2))*(1-(-1)^A000120(n))/2
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* parity of popcount: 1 if odd, 0 if even */
    unsigned long long x = (unsigned long long)n;
    unsigned int parity = 0;
    while (x) {
        parity ^= (x & 1);
        x >>= 1;
    }
    if (parity == 0)
        return 0;
    return ((seq_t)(n / 2) + 1);
}

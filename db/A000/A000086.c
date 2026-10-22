/*
 * A000086  Number of solutions to x^2 - x + 1 == 0 (mod n).
 * https://oeis.org/A000086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   Multiplicative with a(p^e) = 1 if p = 3 and e = 1; 0 if p = 3
 *            and e > 1; 2 if p == 1 (mod 3); 0 if p == 2 (mod 3)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    if (m % 2 == 0) return 0;
    if (m % 3 == 0) {
        m /= 3;
        if (m % 3 == 0) return 0;
    }
    for (p = 5; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (p % 3 == 2) return 0;
            while (m % p == 0) m /= p;
            r *= 2;
        }
    }
    if (m > 1) {
        if (m % 3 == 2) return 0;
        r *= 2;
    }
    return r;
}

/*
 * A000091  Multiplicative with a(2^e) = 2 for k >= 1; a(3) = 2, a(3^e) = 0
 *          for k >= 2; a(p^e) = 0 if p > 3 and p == -1 (mod 3); a(p^e) = 2 if
 *          p > 3 and p == 1 (mod 3).
 * https://oeis.org/A000091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(2^e) = 2 for e >= 1; a(3) = 2, a(3^e) = 0
 *            for e >= 2; a(p^e) = 0 if p == 2 (mod 3), p > 3; a(p^e) = 2 if p
 *            == 1 (mod 3)
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
    idx_t m = n;
    idx_t p;
    int e;

    if (m % 2 == 0) {
        while (m % 2 == 0) m /= 2;
        r *= 2;
    }
    if (m % 3 == 0) {
        e = 0;
        while (m % 3 == 0) { m /= 3; e++; }
        if (e >= 2) return (seq_t)0;
        r *= 2;
    }
    for (p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (p % 3 == 2) return (seq_t)0;
            while (m % p == 0) m /= p;
            r *= 2;
        }
    }
    if (m > 1) {
        if (m % 3 == 2) return (seq_t)0;
        r *= 2;
    }
    return r;
}

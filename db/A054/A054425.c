/*
 * A054425  A054424 expanded to normal triangular array, with zeros at those
 *          (x,y) where x and y are not relatively prime.
 * https://oeis.org/A054425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   T(x,y) = A054424 index of fraction x/y (x+y = r+1, row r,
 *            x=1..r) read by rows; a(n)=0 if gcd(x,y)>1; computed via
 *            Stern-Brocot descent
 * Range:     u8 n <= 36, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A054425_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t r = 1;
    uint64_t rem = (uint64_t)n;
    while (rem > r) {
        rem -= r;
        ++r;
    }
    uint64_t x = rem;
    uint64_t y = r + 1 - x;
    if (A054425_gcd(x, y) != 1) return 0;
    seq_t idx = 1;
    while (x != y) {
        if (x < y) {
            idx = idx * 2;
            y -= x;
        } else {
            idx = idx * 2 + 1;
            x -= y;
        }
    }
    return idx;
}

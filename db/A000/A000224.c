/*
 * A000224  Number of squares mod n.
 * https://oeis.org/A000224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   multiplicative with a(p^e) = floor(p^e/6) + 2 if p = 2;
 *            floor(p^(e+1)/(2p + 2)) + 1 if p > 2
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9384 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a224_local(uint64_t p, uint64_t q)
{
    /* q = p^e */
    if (p == 2)
        return q / 6 + 2;
    return (uint64_t)(((unsigned __int128)q * p) / (2 * (unsigned __int128)p + 2)) + 1;
}

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    if (m <= 1)
        return r;
    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t q = 1;
            while (m % p == 0) {
                m /= p;
                q *= p;
            }
            r *= (seq_t)a224_local(p, q);
        }
    }
    if (m > 1)
        r *= (seq_t)a224_local(m, m);
    return r;
}

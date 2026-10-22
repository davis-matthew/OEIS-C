/*
 * A003050  Number of primitive sublattices of index n in hexagonal lattice:
 *          triples x,y,z from Z/nZ with x+y+z = 0, discarding any triple that
 *          can be obtained from another by multiplying by a unit and
 *          permuting.
 * https://oeis.org/A003050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = psi(n)/6 + C_1/3 + 2^(w-2+C_2), where n =
 *            Product_{i=1..w} p_i^e_i, psi(n) = n*Product (1 + 1/p_i), C_1 =
 *            0 if 2|n or 9|n, else Product_{p_i > 3} (1 + Legendre(p_i, 3)),
 *            C_2 = 2 if n == 0 (mod 8), 1 if n == 1,3,4,5,7 (mod 8), 0 if n
 *            == 2,6 (mod 8). Computed exactly as a(n) = (2*psi(n) + 4*...
 * Range:     u8 n <= 629, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t psi = 1;
    int w = 0;
    int c1_zero = (n % 2 == 0 || n % 9 == 0);
    uint64_t c1 = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t pe = 1;
            while (m % p == 0) {
                m /= p;
                pe *= p;
            }
            psi *= (pe / p) * (p + 1);
            w++;
            if (p > 3) {
                if (p % 3 == 1) c1 *= 2;
                else c1 = 0;
            }
        }
    }
    if (m > 1) {
        psi *= m + 1;
        w++;
        if (m > 3) {
            if (m % 3 == 1) c1 *= 2;
            else c1 = 0;
        }
    }
    if (c1_zero) c1 = 0;

    int c2;
    int r8 = (int)(n % 8);
    if (r8 == 0) c2 = 2;
    else if (r8 == 2 || r8 == 6) c2 = 0;
    else c2 = 1;

    int d = w - 2 + c2;
    uint64_t t; /* 12 * 2^d, d >= -1 */
    if (d < 0) t = 6;
    else t = (uint64_t)12 << d;

    uint64_t q = psi / 6;
    uint64_t r = psi % 6;
    uint64_t extra = 2 * r + 4 * c1 + t;
    return (seq_t)q + (seq_t)(extra / 12);
}

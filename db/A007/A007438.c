/*
 * A007438  Moebius transform of triangular numbers.
 * https://oeis.org/A007438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A007434(n) + A000010(n))/2 = (J_2(n) + phi(n))/2, with
 *            J_2(n) = n^2*Prod_{p|n}(1-1/p^2) and phi(n) =
 *            n*Prod_{p|n}(1-1/p)
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = n;
    uint64_t j2 = 1, ph = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t pk = 1;
            m /= p;
            while (m % p == 0) {
                m /= p;
                pk *= p;
            }
            ph *= pk * (p - 1);
            j2 *= pk * pk * (p * p - 1);
        }
    }
    if (m > 1) {
        ph *= (m - 1);
        j2 *= (m * m - 1);
    }
    return (seq_t)((j2 + ph) / 2);
}

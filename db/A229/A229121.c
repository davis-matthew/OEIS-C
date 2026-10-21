/*
 * A229121  a(n) = Omega(n)^2 - omega(n)^2.
 * https://oeis.org/A229121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Omega(n)^2 - omega(n)^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t temp = (uint64_t)n;
    uint64_t distinct = 0;
    uint64_t lastp = 0;
    seq_t omega = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            if (p != lastp) {
                ++distinct;
                lastp = p;
            }
            do {
                temp /= p;
                ++omega;
            } while (temp % p == 0);
        }
    }

    if (temp > 1) {
        if (temp != lastp) {
            ++distinct;
        }
        ++omega;
    }

    return omega * omega - (seq_t)distinct * (seq_t)distinct;
}

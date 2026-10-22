/*
 * A002652  Theta series of Kleinian lattice Z[(1 + sqrt(-7))/ 2] in 1 complex
 *          (or 2 real) dimensions.
 * https://oeis.org/A002652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; a(n) = 2 * Sum_{d|n} Kronecker(-7, d) for n > 0, where
 *            Kronecker(-7, d) depends on d mod 7: [0, 1, 1, -1, 1, -1, -1]
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   kron_m7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/kron_m7.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    long long s = 0;
    uint64_t d;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            uint64_t e = n / d;
            s += kron_m7(d);
            if (e != d) s += kron_m7(e);
        }
    }
    return (seq_t)(uint64_t)(2 * s);
}

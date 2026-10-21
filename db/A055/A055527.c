/*
 * A055527  Shortest other leg of a Pythagorean triangle with n as length of a
 *          leg.
 * https://oeis.org/A055527
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k > 0 such that n^2 + k^2 is a perfect square
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;          /* behaviour below offset is irrelevant */
    uint64_t nn = (uint64_t)n * n; /* n², fits in 64 bits for the expected n */
    uint64_t best = UINT64_MAX;
    for (uint64_t a = 1; a <= (uint64_t)n; ++a) {
        if (nn % a == 0) {
            uint64_t b = nn / a;
            if ((a & 1) == (b & 1)) {           /* same parity → k integer */
                uint64_t k = b > a ? (b - a) / 2 : (a - b) / 2;
                if (k > 0 && k < best) best = k;
            }
        }
    }
    return (seq_t)best;
}

/*
 * A018226  Magic numbers of nucleons: nuclei with one of these numbers of
 *          either protons or neutrons are more stable against nuclear decay.
 * https://oeis.org/A018226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n*(n+1)*(n+2)/3 for 1 <= n <= 3, a(n) = n*(n^2+5)/3 for n
 *            >= 4
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (n <= 3) {
        return N * (N + 1) * (N + 2) / 3;
    } else {
        return N * (N * N + 5) / 3;
    }
}

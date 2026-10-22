/*
 * A002270  Composite numbers k such that k*sigma(k) == 2 (mod phi(k)).
 * https://oeis.org/A002270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   search for composite k with k*sigma(k) == 2 (mod phi(k)); finite
 *            sequence (4, 6, 22)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   a2270_sig_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2270_sig_phi.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k;
    for (k = 4; k < 2000000ULL; k++) {
        uint64_t s, ph;
        bool prime;
        a2270_sig_phi(k, &s, &ph, &prime);
        if (prime) continue;
        {
            unsigned __int128 prod = (unsigned __int128)k * s;
            uint64_t lhs = (uint64_t)(prod % ph);
            uint64_t rhs = 2 % ph;
            if (lhs == rhs) {
                count++;
                if (count == n) return (seq_t)k;
            }
        }
    }
    return 0;
}

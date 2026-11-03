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
 * Validated: full, pass, 3 terms through n = 3 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void a2270_sig_phi(uint64_t k, uint64_t *sig, uint64_t *ph, bool *prime) {
    uint64_t m = k, s = 1, p = 1, d;
    *prime = true;
    for (d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t pk = 1, ssum = 1;
            *prime = false;
            while (m % d == 0) {
                m /= d;
                pk *= d;
                ssum += pk;
            }
            s *= ssum;
            p *= pk / d * (d - 1);
        }
    }
    if (m > 1) {
        if (m != k) *prime = false;
        s *= (m + 1);
        p *= (m - 1);
    }
    *sig = s;
    *ph = p;
}

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

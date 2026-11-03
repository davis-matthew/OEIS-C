/*
 * A002479  Numbers of the form x^2 + 2*y^2.
 * https://oeis.org/A002479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   Numbers k such that every prime p ≡ 5, 7 (mod 8) dividing k has
 *            even exponent
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9039 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_in_sequence(seq_t m) {
    if (m == 0) return true;
    
    seq_t temp = m;
    seq_t p;
    
    for (p = 2; p * p <= m; p++) {
        if (temp % p == 0) {
            if ((p % 8 == 5) || (p % 8 == 7)) {
                int exp = 0;
                while (temp % p == 0) {
                    exp++;
                    temp /= p;
                }
                if (exp % 2 != 0) {
                    return false;
                }
            } else {
                while (temp % p == 0) {
                    temp /= p;
                }
            }
        }
    }
    
    if (temp > 1) {
        if ((temp % 8 == 5) || (temp % 8 == 7)) {
            return false;
        }
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (true) {
        if (is_in_sequence(m)) {
            count++;
            if (count == n) {
                return m;
            }
        }
        m++;
    }
}

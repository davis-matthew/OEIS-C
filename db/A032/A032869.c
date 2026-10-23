/*
 * A032869  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A032869 – numbers whose base‑6 digits satisfy * d(m) ≥ d(m‑1) ≤
 *            d(m‑2) ≥ … (alternating inequalities)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 1;
    while (1) {
        seq_t tmp = val;
        unsigned char digits[200];
        unsigned int len = 0;
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 6);
            tmp /= 6;
        }
        int ok = 1;
        for (int i = (int)len - 1; i > 0; --i) {
            int require_ge = (((int)len - 1 - i) & 1) == 0;
            if (require_ge) {
                if (digits[i] < digits[i - 1]) { ok = 0; break; }
            } else {
                if (digits[i] > digits[i - 1]) { ok = 0; break; }
            }
        }
        if (ok) {
            ++count;
            if (count == n) return val;
        }
        ++val;
    }
}

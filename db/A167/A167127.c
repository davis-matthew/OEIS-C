/*
 * A167127  Cubes that may be transformed into squares by prefixing with a
 *          single nonzero digit.
 * https://oeis.org/A167127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = BASE[(n-1) mod 4] * 10^(6 * floor((n-1)/4)) BASE = {1,
 *            216, 68921, 884736} (1^3, 6^3, 41^3, 96^3)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 */
        return 0;
    static const seq_t base[4] = {1, 216, 68921, 884736};
    idx_t offset = (n - 1) % 4;
    idx_t k      = (n - 1) / 4;
    seq_t mult   = pow10(6 * k);
    return base[offset] * mult;
}

/*
 * A094904  Least hypotenuse of primitive Pythagorean triangles exceeding a
 *          leg by A096033(n).
 * https://oeis.org/A094904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k+1)^2+1 if A096033(n)=k^2 (k odd); a(n) = m^2+(m+1)^2
 *            if A096033(n)=2*m^2 (c-leg = (m'-n')^2 or 2n'^2 for primitive
 *            triple)
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t odd_k = 0, even_m = 1, count = 0;
    seq_t result = 0;
    while (count < n) {
        seq_t k = (seq_t)(2 * odd_k + 1);
        seq_t m = (seq_t)even_m;
        seq_t odd_val = k * k;
        seq_t even_val = (seq_t)2 * m * m;
        if (odd_val <= even_val) {
            result = (k + 1) * (k + 1) + 1;
            odd_k++;
        } else {
            result = m * m + (m + 1) * (m + 1);
            even_m++;
        }
        count++;
    }
    return result;
}

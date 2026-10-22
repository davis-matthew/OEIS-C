/*
 * A003894  Degrees of irreducible representations of group L3(3).
 * https://oeis.org/A003894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   finite sequence (12 terms). With q = 3, the irreducible
 *            character degrees of L3(3) = PSL(3,q) are 1, q(q+1), q^2+q+1,
 *            (q-1)^2(q+1) [4 times], (q-1)(q^2+q+1) [3 times], q^3,
 *            q(q^2+q+1), listed in increasing order; a(n) = 0 for n > 12
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t q = (seq_t)3;
    seq_t d1 = (seq_t)1;
    seq_t d2 = q * (q + 1);
    seq_t d3 = q * q + q + 1;
    seq_t d4 = (q - 1) * (q - 1) * (q + 1);
    seq_t d5 = (q - 1) * (q * q + q + 1);
    seq_t d6 = q * q * q;
    seq_t d7 = q * (q * q + q + 1);
    if (n == 1) return d1;
    if (n == 2) return d2;
    if (n == 3) return d3;
    if (n >= 4 && n <= 7) return d4;
    if (n >= 8 && n <= 10) return d5;
    if (n == 11) return d6;
    if (n == 12) return d7;
    return (seq_t)0;
}

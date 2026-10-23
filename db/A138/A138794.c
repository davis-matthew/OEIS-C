/*
 * A138794  a(n) = A138793(n+1)-A138793(n).
 * https://oeis.org/A138794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+1)*10^n for n <= 8 * a(n) = (10n-89)*10^(2n-9) for n
 *            >= 9 * (OEIS A138794 – first differences of A138793)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 8u) {
        seq_t coeff = (seq_t)n + 1;
        return coeff * pow10((idx_t)n);
    } else {
        seq_t coeff = (seq_t)10 * (seq_t)n - (seq_t)89;
        idx_t exp   = 2 * n - 9;
        return coeff * pow10(exp);
    }
}

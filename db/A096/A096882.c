/*
 * A096882  Expansion of g.f. (1 + 7*x)/(1 - 50*x^2).
 * https://oeis.org/A096882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 50^{⌊n/2⌋} if n is even * = 7·50^{⌊n/2⌋} if n is odd * *
 *            This follows directly from the generating function *
 *            (1+7x)/(1-50x^2) = Σ_{m≥0} 50^m x^{2m} + 7·Σ_{m≥0} 50^m x^{2m+1}
 *            . * It gives the exact closed‑form values and avoids any
 *            recurrence
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow50
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow50.h"

seq_t a(idx_t n) {
    idx_t half = n / 2;        /* ⌊n/2⌋                                     */
    seq_t val   = pow50(half); /* 50^{⌊n/2⌋}                                */
    if (n & 1) val *= 7;       /* odd n → multiply by 7                      */
    return val;
}

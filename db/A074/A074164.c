/*
 * A074164  Smallest k such that R(k) > n*k, where R(k) is the digit reversal
 *          of k (A004086) (the reversal of 10 is taken to be 01 = 1, etc.).
 * https://oeis.org/A074164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = smallest k such that R(k) > n*k R(k) is the decimal digit
 *            reversal of k
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* No solution exists for n ≥ 10 because R(k) < 10*k for all k. */
    if (n >= 10) return 0;

    seq_t sn = (seq_t)n;
    seq_t k   = 1;

    for (;; ++k)
    {
        seq_t rk = reverse_decimal(k);
        if (rk > sn * k) return k;
    }
}

/*
 * A228636  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 10.
 * https://oeis.org/A228636
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f_min(10)(n) - f_min(11)(n) where f_min(L)(n) =
 *            Σ_{k=1}^{⌊n/L⌋} C(n - L*k + k - 1, k - 1) * n * (n-1)^(k-1)
 * Range:     u8 n <= 19, u16 n <= 30, u32 n <= 51, u64 n <= 87, u128 n <= 155,
 *            big any n
 * Helpers:   binom_comb, f_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_comb.h"
#include "../utils/f_min.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 10) return 0;          /* offset behaviour */

    seq_t f10 = f_min(n, 10);
    seq_t f11 = f_min(n, 11);
    return f10 - f11;
}

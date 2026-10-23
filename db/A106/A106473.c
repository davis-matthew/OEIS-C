/*
 * A106473  Rows of A003989 expressed as base 10 numbers.
 * https://oeis.org/A106473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum{k=0..n, GCD(n-k+1, k+1)*10^k}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t pow10   = (seq_t)1;          /* 10^0 */
    idx_t k;

    for (k = (idx_t)0; k <= n; ++k) {
        idx_t a_val = n - k + (idx_t)1;
        idx_t b_val = k + (idx_t)1;

        /* gcd of the two parts, using 64‑bit routine */
        idx_t g = gcd_uint64(a_val, b_val);

        /* current term = g * 10^k */
        seq_t term = (seq_t)g * pow10;
        result += term;

        /* advance 10^k to 10^(k+1) */
        pow10 = pow10 * ((seq_t)10);
    }

    return result;
}

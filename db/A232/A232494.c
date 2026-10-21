/*
 * A232494  If n mod 2 = 0 then 2^(n-1)*(3^n+3*3^(n/2)-2) otherwise
 *          2^(n-1)*(3^n+5*3^((n-1)/2)-2).
 * https://oeis.org/A232494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) * (3^n + 3*3^(n/2) - 2) if n is even, a(n) =
 *            2^(n-1) * (3^n + 5*3^((n-1)/2) - 2) if n is odd. with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t pow2_n_minus1 = pow2(n - 1);
    seq_t pow3_n = pow3(n);

    seq_t multiplier;
    if ((n & 1) == 0) { /* n even */
        seq_t pow3_half = pow3(n / 2);
        multiplier = pow3_n + pow3_half * (seq_t)3 - (seq_t)2;
    } else {           /* n odd */
        seq_t pow3_half = pow3((n - 1) / 2);
        multiplier = pow3_n + pow3_half * (seq_t)5 - (seq_t)2;
    }

    return pow2_n_minus1 * multiplier;
}

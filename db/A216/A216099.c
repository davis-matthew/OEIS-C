/*
 * A216099  Period of powers of 3 mod 10^n.
 * https://oeis.org/A216099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*5^(n-1) for n ≤ 4, * a(n) = 5*10^(n-2) for n ≥ 5 * (and
 *            equivalently a(n) = 10*a(n-1) for n > 4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow10, power5, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        seq_t v = power5(n - 1);          /* 5^(n-1)  */
        return seq_mul(v, (seq_t)4);      /* 4*5^(n-1) */
    } else {
        seq_t v = pow10(n - 2);           /* 10^(n-2) */
        return seq_mul(v, (seq_t)5);      /* 5*10^(n-2) */
    }
}

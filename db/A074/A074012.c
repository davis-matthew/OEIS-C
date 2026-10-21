/*
 * A074012  Number of elements of GF(5^n) with trace 1 and subtrace 3.
 * https://oeis.org/A074012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   * a(n) = 5^(n-2) + s * 5^((n-2)/2) * where * s = -1 if n % 5 ==
 *            0 (simply a(n) = 5^(n-2)) * s = -1 if n % 5 == 1 or 4 * s =
 *            (-1)^(floor(n/5)+1) if n % 5 == 2 * s = (-1)^floor(n/5) if n % 5
 *            == 3 * For n < 3, a(n) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    idx_t exp_n2 = n - 2;
    seq_t pow_n2 = big_pow((seq_t)5, (seq_t)exp_n2);

    idx_t exp_e = (n - 2) / 2;
    seq_t pow_e = big_pow((seq_t)5, (seq_t)exp_e);

    idx_t r = n % 5;
    if (r == 0) return pow_n2;          /* a(n) = 5^(n-2) when n ≡ 0 (mod 5) */

    int sign;
    if (r == 1 || r == 4) {
        sign = -1;
    } else if (r == 2) {                /* sign = (-1)^(floor(n/5)+1) */
        idx_t k = n / 5;
        sign = ((k + 1) & 1) ? -1 : 1;
    } else {                             /* r == 3, sign = (-1)^floor(n/5) */
        idx_t k = n / 5;
        sign = (k & 1) ? -1 : 1;
    }

    if (sign > 0) return pow_n2 + pow_e;
    else          return pow_n2 - pow_e;
}

/*
 * A107660  Sum 3^max(k,n-k),k=0..n.
 * https://oeis.org/A107660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum 3^max(k,n-k), k=0..n. Closed form used: a(2n) =
 *            3^(2n+1) - 2*3^n a(2n+1) = 9^(n+1) - 3^(n+1) = 3^(2(n+1)) -
 *            3^(n+1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow3_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3_exp.h"

/* Sequence A107660 */
seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)          /* even n = 2*m */
    {
        idx_t m      = n / 2;
        seq_t pow_n  = pow3_exp(m);
        seq_t pow_2n1 = pow3_exp((idx_t)(2 * m + 1));
        return pow_2n1 - ((seq_t)2 * pow_n);
    }
    else                            /* odd n = 2*m+1 */
    {
        idx_t m      = (n - 1) / 2;
        seq_t pow_2m2 = pow3_exp((idx_t)(2 * (m + 1)));
        seq_t pow_m1  = pow3_exp(m + 1);
        return pow_2m2 - pow_m1;
    }
}

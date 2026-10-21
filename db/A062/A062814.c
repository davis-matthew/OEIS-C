/*
 * A062814  a(n) = Sum_{i=0..n-1} i * (n - i)^(n - i).
 * https://oeis.org/A062814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0}^{n-1} i * (n-i)^(n-i)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_seq_t_manual
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t_manual.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) = 0 */

    seq_t a_prev     = (seq_t)0;     /* a(1) */
    seq_t powerSum   = (seq_t)0;     /* sum_{j=1}^{k-1} j^j, starts empty */

    for (idx_t k = 2; k <= n; ++k)
    {
        idx_t   exp   = k - 1;                  /* exponent (k-1) */
        seq_t   base  = (seq_t)exp;             /* base (k-1) */
        seq_t   power = pow_seq_t_manual(base, exp); /* (k-1)^(k-1) */

        seq_t a_new = a_prev + powerSum + power;   /* a(k) */
        powerSum   += power;                       /* update sum_{j=1}^{k-1} */
        a_prev      = a_new;
    }

    return a_prev;   /* a(n) */
}

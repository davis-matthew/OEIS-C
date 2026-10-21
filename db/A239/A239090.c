/*
 * A239090  The sequence S = a(1), a(2), ... is defined by a(1)=1, if d,e,f
 *          are consecutive digits then we do not have d >= e < f, and S is
 *          always extended with the smallest integer not yet present in S.
 * https://oeis.org/A239090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for 1 <= n <= 9, else ((10^(n-8) - 1) / 9)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 9u) {
        return (seq_t)n;
    }
    idx_t e = n - (idx_t)8;
    seq_t p = pow10(e);   /* 10^(n-8) as seq_t */
    return (p - (seq_t)1) / (seq_t)9;
}
